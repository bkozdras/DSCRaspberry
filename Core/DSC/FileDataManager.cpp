#include "FileDataManager.hpp"

#include <boost/tokenizer.hpp>

#include "../Defines/CommonDefines.hpp"
#include "../Utilities/Logger.hpp"
#include "../Utilities/ToStringConverter.hpp"
#include "../FaultManagement/FaultManager.hpp"
#include "HeaterManager.hpp"
#include "../ModelIdentification/EInputDataSource.hpp"
#include "../ModelIdentification/ExperimentManager.hpp"
#include "../SharedDefines/EUnitId.h"

#include <algorithm>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <locale>
#include <sstream>

namespace DSC
{
    bool FileDataManager::initialize()
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);

        mIsNewAttributeCallbackRegistered = false;
        mIsDscDataRegisteringActive = false;
        mIsHeaterPowerControlDataRegisteringActive = false;

        auto result = createDataDirectoryIfDoesNotExist();

        if (result)
        {
            Logger::info("%s: Initialized!", getLoggerPrefix().c_str());
        }
        else
        {
            Logger::error("%s: Not initialized...", getLoggerPrefix().c_str());
        }

        return result;
    }

    void FileDataManager::startRegisteringHeaterControlSystemData()
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        ifFileExistsChangeName(mHeaterControlSystemDataDirectoryPath, std::string("HeaterPowerControlDataFileName"));
        _startRegisteringHeaterControlSystemData();
    }

    void FileDataManager::stopRegisteringHeaterControlSystemData()
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        _stopRegisteringHeaterControlSystemData();
    }

    void FileDataManager::startRegisteringDscData()
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        ifFileExistsChangeName(mDscDataDirectoryPath, std::string("DscDataFileName"));
        _startRegisteringDscData();
    }

    void FileDataManager::stopRegisteringDscData()
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        _stopRegisteringDscData();
    }

    void FileDataManager::_startRegisteringHeaterControlSystemData()
    {
        std::string fileName;
        DSC::DataManager::getUnitAttribute(EUnitId_Raspberry, "HeaterPowerControlDataFileName", fileName);
        mHeaterControlSystemDataFilePath = mHeaterControlSystemDataDirectoryPath;
        mHeaterControlSystemDataFilePath /= fileName;

        if (openFile(mHeaterDataFileStream, mHeaterControlSystemDataFilePath))
        {
            mControlMode = DSC::DataManager::getControlMode();
            mHeaterDataSampleNumber = 0U;

            saveToFileAuthor(mHeaterDataFileStream);
            saveToFileCurrentData(mHeaterDataFileStream);
            saveToFileCurrentControlMode(mHeaterDataFileStream);
            saveToFileRegisteringPeriod(mHeaterDataFileStream);
            saveToFileLegend(mHeaterDataFileStream);
			saveToFileExperimentInfoIfIsInRunningState(mHeaterDataFileStream);
			saveToFileDescribeData(mHeaterDataFileStream);

            mNewControlModeCallbackId = DSC::DataManager::registerNewControlModeCallback
            (
                [](EControlMode mode)
                {
                    newControlModeCallback(mode);
                }
            );

            if (!mIsNewAttributeCallbackRegistered)
            {
                mNewAttributeCallbackId = DSC::DataManager::registerNewUnitAttributeCallback
                (
                    [](EUnitId unitId, const std::string & attribute, const std::string & value)
                    {
                        newAtributeCallback(unitId, attribute, value);
                    }
                );

                mIsNewAttributeCallbackRegistered = true;
            }

            auto registeringPeriod = static_cast<uint64_t>(1000.0 * DSC::DataManager::getData(EDataType::HeaterPowerControlFileDataSampling));
            mHeaterControlSystemStoreDataCallbackId = TimerManager::create
            (
                registeringPeriod,
                registeringPeriod,
                []()
                {
                    storeHeaterControlSystemDataCallback();
                }
            );

            Logger::info("%s: Started registering heater control system data with period: %u milliseconds.", getLoggerPrefix().c_str(), registeringPeriod);
            Logger::info("%s: Path to file: %s.", getLoggerPrefix().c_str(), mHeaterControlSystemDataFilePath.c_str());

            mIsHeaterPowerControlDataRegisteringActive = true;
        }
        else
        {
            Logger::error("%s: Starting registering heater control system data failed...", getLoggerPrefix().c_str());
        }
    }

    void FileDataManager::_stopRegisteringHeaterControlSystemData()
    {
        if (mIsHeaterPowerControlDataRegisteringActive)
        {
            if (closeFile(mHeaterDataFileStream))
            {
                TimerManager::destroy(mHeaterControlSystemStoreDataCallbackId);
                DSC::DataManager::deregisterNewControlModeCallback(mNewControlModeCallbackId);

                mIsHeaterPowerControlDataRegisteringActive = false;
                if (!mIsDscDataRegisteringActive)
                {
                    DSC::DataManager::deregisterNewUnitAttributeCallback(mNewAttributeCallbackId);
                    mIsNewAttributeCallbackRegistered = false;
                }

                Logger::info("%s: Stopped registering heater control system data.", getLoggerPrefix().c_str());
                Logger::info("%s: Saved file: %s.", getLoggerPrefix().c_str(), mHeaterControlSystemDataFilePath.c_str());
            }
        }
    }

    void FileDataManager::_startRegisteringDscData()
    {
        std::string fileName;
        DSC::DataManager::getUnitAttribute(EUnitId_Raspberry, "DscDataFileName", fileName);
        mDscDataFilePath = mDscDataDirectoryPath;
        mDscDataFilePath /= fileName;

        if (openFile(mDscDataFileStream, mDscDataFilePath))
        {
            mDscDataSampleNumber = 0U;

            saveToFileAuthor(mDscDataFileStream);
            saveToFileCurrentData(mDscDataFileStream);
            saveToFileRegisteringPeriod(mDscDataFileStream);
            saveToFileLegend(mDscDataFileStream);
            saveToFileDescribeData(mDscDataFileStream);

            if (!mIsNewAttributeCallbackRegistered)
            {
                mNewAttributeCallbackId = DSC::DataManager::registerNewUnitAttributeCallback
                (
                    [](EUnitId unitId, const std::string & attribute, const std::string & value)
                    {
                        newAtributeCallback(unitId, attribute, value);
                    }
                );

                mIsNewAttributeCallbackRegistered = true;
            }

            auto registeringPeriod = static_cast<uint64_t>(1000.0 * DSC::DataManager::getData(EDataType::DscDataFileDataSampling));
            mDscStoreDataCallbackId = TimerManager::create
            (
                registeringPeriod,
                registeringPeriod,
                []()
                {
                    storeDscDataCallback();
                }
            );

            mIsDscDataRegisteringActive = true;

            Logger::info("%s: Started registering DSC data with period: %u milliseconds.", getLoggerPrefix().c_str(), registeringPeriod);
            Logger::info("%s: Path to file: %s.", getLoggerPrefix().c_str(), mDscDataFilePath.c_str());
        }
        else
        {
            Logger::error("%s: Starting registering DSC data failed...", getLoggerPrefix().c_str());
        }
    }

    void FileDataManager::_stopRegisteringDscData()
    {
        if (mIsDscDataRegisteringActive)
        {
            if (closeFile(mDscDataFileStream))
            {
                TimerManager::destroy(mDscStoreDataCallbackId);

                mIsDscDataRegisteringActive = false;
                if (!mIsHeaterPowerControlDataRegisteringActive)
                {
                    DSC::DataManager::deregisterNewUnitAttributeCallback(mNewAttributeCallbackId);
                    mIsNewAttributeCallbackRegistered = false;
                }

                Logger::info("%s: Stopped registering DSC data.", getLoggerPrefix().c_str());
                Logger::info("%s: Saved file: %s.", getLoggerPrefix().c_str(), mDscDataFilePath.c_str());
            }
        }
    }

    void FileDataManager::newAtributeCallback(EUnitId unitId, const std::string & attribute, const std::string & value)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);

        if (EUnitId_Raspberry == unitId)
        {
            if ("HeaterControlSystemDataFilename" == attribute)
            {
                _stopRegisteringHeaterControlSystemData();
                _startRegisteringHeaterControlSystemData();
            }
            else if ("DSCDataFilename" == attribute)
            {
                _stopRegisteringDscData();
                _startRegisteringDscData();
            }
        }
    }

    void FileDataManager::newControlModeCallback(EControlMode mode)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);

        mControlMode = mode;

        _stopRegisteringHeaterControlSystemData();

        if (EControlMode::NotSet != mode)
        {
            _startRegisteringHeaterControlSystemData();
        }
    }

    void FileDataManager::storeHeaterControlSystemDataCallback()
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);

        auto time = getActualTime();
        auto sampleNumber = ++mHeaterDataSampleNumber;
        
        mHeaterDataFileStream << time;
        mHeaterDataFileStream << "\t";
        mHeaterDataFileStream << sampleNumber;
        mHeaterDataFileStream << "\t";

        switch (mControlMode)
        {
            case EControlMode::OpenLoop:
            {
				auto cv1Value = HeaterManager::getPowerInPercent();
				auto cv2Value = HeaterManager::getPower();
                auto pv1Value = DSC::DataManager::getData(EDataType::HeaterTemperature);
				auto pv2Value = DSC::DataManager::getData(EDataType::HeaterTemperatureRtdResistance);

                mHeaterDataFileStream << convertDoubleToString(cv1Value, 2);
                mHeaterDataFileStream << "\t";
				mHeaterDataFileStream << convertDoubleToString(cv2Value, 2);
				mHeaterDataFileStream << "\t";
                mHeaterDataFileStream << convertDoubleToString(pv1Value, 2);
				mHeaterDataFileStream << "\t";
				mHeaterDataFileStream << convertDoubleToString(pv2Value, 2);

                break;
            }

            case EControlMode::SimpleFeedback :
            case EControlMode::MFCFeedback:
            {
                auto spValue = DSC::DataManager::getData(EDataType::SPHeaterTemperature);
                auto cvValue = DSC::DataManager::getData(EDataType::CVHeaterTemperature);
                auto pvValue = DSC::DataManager::getData(EDataType::HeaterTemperature);
                auto errValue = DSC::DataManager::getData(EDataType::ERRHeaterTemperature);

                mHeaterDataFileStream << convertDoubleToString(spValue, 2);
                mHeaterDataFileStream << "\t";
                mHeaterDataFileStream << convertDoubleToString(cvValue, 2);
                mHeaterDataFileStream << "\t";
                mHeaterDataFileStream << convertDoubleToString(pvValue, 2);
                mHeaterDataFileStream << "\t";
                mHeaterDataFileStream << convertDoubleToString(errValue, 2);

                break;
            }

            default :
                break;
        }

        mHeaterDataFileStream << std::endl;
    }

    void FileDataManager::storeDscDataCallback()
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);

        auto time = getActualTime();
        auto sampleNumber = ++mDscDataSampleNumber;

        mDscDataFileStream << time;
        mDscDataFileStream << "\t";
        mDscDataFileStream << sampleNumber;
        mDscDataFileStream << "\t";

        auto heaterSpValue = DSC::DataManager::getData(EDataType::SPHeaterTemperature);
        auto heaterPvValue = DSC::DataManager::getData(EDataType::HeaterTemperature);
        auto heaterCvValue = DSC::DataManager::getData(EDataType::HeaterPower);
        auto heaterErrValue = DSC::DataManager::getData(EDataType::ERRHeaterTemperature);
        auto sampleCarrierTemperatue = DSC::DataManager::getData(EDataType::SampleCarrierTemperature);
        auto sample1Value = DSC::DataManager::getData(EDataType::Thermocouple1);
        auto sample2Value = DSC::DataManager::getData(EDataType::Thermocouple2);
        auto sample3Value = DSC::DataManager::getData(EDataType::Thermocouple3);
        auto sample4Value = DSC::DataManager::getData(EDataType::Thermocouple4);
        auto sample1FilteredValue = DSC::DataManager::getData(EDataType::FilteredThermocouple1);
        auto sample2FilteredValue = DSC::DataManager::getData(EDataType::FilteredThermocouple2);
        auto sample3FilteredValue = DSC::DataManager::getData(EDataType::FilteredThermocouple3);
        auto sample4FilteredValue = DSC::DataManager::getData(EDataType::FilteredThermocouple4);

        auto referenceThermocouple = DSC::DataManager::getData(EDataType::ReferenceThermocouple);
        auto smPcbTemp = DSC::DataManager::getData(EDataType::SMPCBTemperature);

        mDscDataFileStream << convertDoubleToString(heaterSpValue);
        mDscDataFileStream << "\t";
        mDscDataFileStream << convertDoubleToString(heaterPvValue);
        mDscDataFileStream << "\t";
        mDscDataFileStream << convertDoubleToString(heaterCvValue);
        mDscDataFileStream << "\t";
        mDscDataFileStream << convertDoubleToString(heaterErrValue);
        mDscDataFileStream << "\t";
        mDscDataFileStream << convertDoubleToString(sampleCarrierTemperatue);
        mDscDataFileStream << "\t";
        mDscDataFileStream << convertDoubleToString(sample1Value, 3);
        mDscDataFileStream << "\t";
        mDscDataFileStream << convertDoubleToString(sample2Value, 3);
        mDscDataFileStream << "\t";
        mDscDataFileStream << convertDoubleToString(sample3Value, 3);
        mDscDataFileStream << "\t";
        mDscDataFileStream << convertDoubleToString(sample4Value, 3);
        mDscDataFileStream << "\t";
        mDscDataFileStream << convertDoubleToString(sample1FilteredValue, 3);
        mDscDataFileStream << "\t";
        mDscDataFileStream << convertDoubleToString(sample2FilteredValue, 3);
        mDscDataFileStream << "\t";
        mDscDataFileStream << convertDoubleToString(sample3FilteredValue, 3);
        mDscDataFileStream << "\t";
        mDscDataFileStream << convertDoubleToString(sample4FilteredValue, 3);

        mDscDataFileStream << "\t";
        mDscDataFileStream << convertDoubleToString(referenceThermocouple, 3);

        mDscDataFileStream << "\t";
        mDscDataFileStream << convertDoubleToString(smPcbTemp);

        mDscDataFileStream << std::endl;
    }

    bool FileDataManager::createDataDirectoryIfDoesNotExist()
    {
        auto currentPath = boost::filesystem::current_path();
        Logger::info("%s: Current path: %s.", getLoggerPrefix().c_str(), currentPath.c_str());

        mHeaterControlSystemDataDirectoryPath = currentPath;
        mHeaterControlSystemDataDirectoryPath /= "HeaterData";

        if (!boost::filesystem::exists(mHeaterControlSystemDataDirectoryPath))
        {
            Logger::info("%s: Directory for collecting heater data does not exist.", getLoggerPrefix().c_str());
            if (boost::filesystem::create_directory(mHeaterControlSystemDataDirectoryPath))
            {
                Logger::info("%s: Created directory: %s.", getLoggerPrefix().c_str(), mHeaterControlSystemDataDirectoryPath.c_str());
            }
            else
            {
                Logger::error("%s: Creating directory: %s failed!", getLoggerPrefix().c_str(), mHeaterControlSystemDataDirectoryPath.c_str());
                FaultManager::generate(EFaultId_System, EUnitId_Raspberry);
                return false;
            }
        }
        else
        {
            Logger::info("%s: Directory for collecting heater data exists.", getLoggerPrefix().c_str());
        }

        mDscDataDirectoryPath = currentPath;
        mDscDataDirectoryPath /= "DSCData";

        if (!boost::filesystem::exists(mDscDataDirectoryPath))
        {
            Logger::info("%s: Directory for collecting DSC data does not exist.", getLoggerPrefix().c_str());
            if (boost::filesystem::create_directory(mDscDataDirectoryPath))
            {
                Logger::info("%s: Created directory: %s.", getLoggerPrefix().c_str(), mDscDataDirectoryPath.c_str());
            }
            else
            {
                Logger::error("%s: Creating directory: %s failed!", getLoggerPrefix().c_str(), mDscDataDirectoryPath.c_str());
                FaultManager::generate(EFaultId_System, EUnitId_Raspberry);
                return false;
            }
        }
        else
        {
            Logger::info("%s: Directory for collecting DSC exists.", getLoggerPrefix().c_str());
        }

        return true;
    }

    void FileDataManager::ifFileExistsChangeName(const boost::filesystem::path & directoryPath, const std::string & fileNameAttribute)
    {
        std::string fileName;
        DSC::DataManager::getUnitAttribute(EUnitId_Raspberry, fileNameAttribute, fileName);
        
        auto filePath = directoryPath;
        filePath /= fileName;

        if (!boost::filesystem::exists(filePath))
        {
            return;
        }

        boost::char_separator<char> separator(".");
        boost::tokenizer<boost::char_separator<char>> tokens(fileName, separator);

        std::string fileNamePrefix = "";
        std::string fileNamePostfix = "";
        std::vector<std::string> splitFileName(std::begin(tokens), std::end(tokens));
        
        if (splitFileName.empty())
        {
            return;
        }

        if (1 == splitFileName.size())
        {
            fileNamePrefix = splitFileName[0];
            fileNamePostfix = "";
        }
        else
        {
            for (decltype(splitFileName.size()) iter = 0; (splitFileName.size() - 1) > iter; ++iter)
            {
                fileNamePrefix += splitFileName[iter];
                if (2 < splitFileName.size() && iter != (splitFileName.size() - 2))
                {
                    fileNamePrefix += ".";
                }
            }

            fileNamePostfix = splitFileName.back();
        }

        auto number = 0;
        std::string newFileName;

        do
        {
            filePath = directoryPath;
            newFileName = fileNamePrefix + "_" + std::to_string(++number) + "." + fileNamePostfix;
            filePath /= newFileName;
        } while (boost::filesystem::exists(filePath));

        DSC::DataManager::updateUnitAttribute(EUnitId_Raspberry, fileNameAttribute, newFileName);
    }

    bool FileDataManager::openFile(std::ofstream & fileStream, const boost::filesystem::path & filePath)
    {
        if (!fileStream.is_open())
        {
            try
            {
                fileStream.open(filePath.string(), std::ios::out | std::ios::app);
            }
            catch (std::ios_base::failure failure)
            {
                std::string fileName;
                if (mHeaterDataFileStream == fileStream)
                {
                    fileName = mHeaterControlSystemDataFilePath.string();
                }
                else
                {
                    fileName = mDscDataFilePath.string();
                }

                Logger::error("%s: Opening file: %s failed.", getLoggerPrefix().c_str(), fileName.c_str());
                Logger::error("%s: %s.", getLoggerPrefix().c_str(), failure.what());
                FaultManager::generate(EFaultId_System, EUnitId_Raspberry);

                return false;
            }
        }

        return true;
    }

    bool FileDataManager::closeFile(std::ofstream & fileStream)
    {
        if (fileStream.is_open())
        {
            try
            {
                fileStream.close();
            }
            catch (std::ios_base::failure failure)
            {
                std::string fileName;
                if (mHeaterDataFileStream == fileStream)
                {
                    fileName = mHeaterControlSystemDataFilePath.string();
                }
                else
                {
                    fileName = mDscDataFilePath.string();
                }

                Logger::error("%s: Closing file: %s failed.", getLoggerPrefix().c_str(), fileName.c_str());
                Logger::error("%s: %s.", getLoggerPrefix().c_str(), failure.what());
                FaultManager::generate(EFaultId_System, EUnitId_Raspberry);
                
                return false;
            }
        }

        return true;
    }

    void FileDataManager::saveToFileAuthor(std::ofstream & fileStream)
    {
        fileStream << std::endl;
        fileStream << "File generated automatically by DSC Application." << std::endl;
        fileStream << "DSC Application: @bkozdras (Bartlomiej Kozdras)." << std::endl;
        fileStream << "@mail: b.kozdras@gmail.com" << std::endl;
        fileStream << std::endl;
    }

    void FileDataManager::saveToFileCurrentData(std::ofstream & fileStream)
    {
        fileStream << "Date: " << getActualDate() << ". Weekday: " << getActualWeekday() << ". Time: " << getActualTime() << "." << std::endl;
    }

    void FileDataManager::saveToFileCurrentControlMode(std::ofstream & fileStream)
    {
        std::string controlModeStr;
        switch (mControlMode)
        {
            case EControlMode::NotSet:
            {
                controlModeStr = "Not Set";
                break;
            }

            case EControlMode::OpenLoop:
            {
                controlModeStr = "Open Loop";
                break;
            }

            case EControlMode::SimpleFeedback:
            {
                controlModeStr = "Simple Feedback";
                break;
            }

            case EControlMode::MFCFeedback:
            {
                controlModeStr = "MFC Feedback";
                break;
            }
            
            default :
                break;
        }

        fileStream << "Control Mode: " << controlModeStr << "." << std::endl;
    }

    void FileDataManager::saveToFileLegend(std::ofstream & fileStream)
    {
        fileStream << std::endl;

        fileStream << "Time - when data collected [hh::mm::ss]" << std::endl;
        fileStream << "Sample Number - data number" << std::endl;

        if (mHeaterDataFileStream == fileStream)
        {
            if (EControlMode::OpenLoop == mControlMode)
            {
                fileStream << "Power - CV1 - heater power [%] (0 - 100 %)" << std::endl;
				fileStream << "Power - CV2 - heater power [DAC] (0 - 1000)" << std::endl;
                fileStream << "Temperature - PV1 - heater temperature [oC]" << std::endl;
				fileStream << "RTD - PV2 - Pt1000 (RTD) resistance related to heater temperature [ohm]" << std::endl;

            }
            else
            {
                fileStream << "SP - ideal heater temperature (Set Point) [oC]" << std::endl;
                fileStream << "PV - real heater temperature (Process Value) [oC]" << std::endl;
                fileStream << "CV - heater power (Control Value) [%]" << std::endl;
                fileStream << "ERR - heater temperature error (ERR = SP - PV) [oC]" << std::endl;
            }
        }
        else
        {
            fileStream << "SP - ideal heater temperature (Set Point) [oC]" << std::endl;
            fileStream << "PV - real heater temperature (Process Value) [oC]" << std::endl;
            fileStream << "CV - heater power (Control Value) [%]" << std::endl;
            fileStream << "ERR - heater temperature error (ERR = SP - PV) [oC]" << std::endl;
            fileStream << "SC - sample carrier temperature [oC]" << std::endl;
            fileStream << "S1 - sample 1 effect voltage value [uV]" << std::endl;
            fileStream << "S2 - sample 2 effect voltage value [uV]" << std::endl;
            fileStream << "S3 - sample 3 effect voltage value [uV]" << std::endl;
            fileStream << "S4 - sample 4 effect voltage value [uV]" << std::endl;
            fileStream << "SF1 - sample 1 effect filtered voltage value [uV]" << std::endl;
            fileStream << "SF2 - sample 2 effect filtered voltage value [uV]" << std::endl;
            fileStream << "SF3 - sample 3 effect filtered voltage value [uV]" << std::endl;
            fileStream << "SF4 - sample 4 effect filtered voltage value [uV]" << std::endl;
        }

        fileStream << std::endl;
    }

    void FileDataManager::saveToFileRegisteringPeriod(std::ofstream & fileStream)
    {
        double registeringPeriod;

        if (mHeaterDataFileStream == fileStream)
        {
            registeringPeriod = DSC::DataManager::getData(EDataType::HeaterPowerControlFileDataSampling);
        }
        else if (mDscDataFileStream == fileStream)
        {
            registeringPeriod = DSC::DataManager::getData(EDataType::DscDataFileDataSampling);
        }
        else
        {
            return;
        }

        registeringPeriod *= 1000.0;

        auto str = convertDoubleToString(registeringPeriod, 2);
        fileStream << "Data registering period: " << str << " milliseconds." << std::endl;
    }

    void FileDataManager::saveToFileDescribeData(std::ofstream & fileStream)
    {
        fileStream << std::endl;
        fileStream << "Time" << "\t" << "Sample Number" << "\t";

        if (mHeaterDataFileStream == fileStream)
        {
            if (EControlMode::OpenLoop == mControlMode)
            {
                fileStream << "CV1" << "\t" << "CV2" << "\t" << "PV1" << "\t" << "PV2";
            }
            else
            {
                fileStream << "SP" << "\t" << "CV" << "\t" << "PV" << "\t" << "ERR";
            }
        }
        else
        {
            fileStream << "SP" << "\t" << "PV" << "\t" << "CV" << "\t" << "ERR" << "\t" << "SC" << "\t" << "S1" << "\t" << "S2" << "\t" << "S3" << "\t" << "S4" << "\t" << "SF1" << "\t" << "SF2" << "\t" << "SF3" << "\t" << "SF4";
        }

        fileStream << std::endl << std::endl;
    }

	void FileDataManager::saveToFileExperimentInfoIfIsInRunningState(std::ofstream & fileStream)
	{
		const auto experimentState = ModelIdentification::ExperimentManager::getExperimentState();
		if (EExperimentState::Running != experimentState)
		{
			return;
		}
		
		fileStream << std::endl << "Model Identification Experiment" << std::endl;

		const auto inputDataSource = ModelIdentification::ExperimentManager::getInputDataSource();
		fileStream << "Input data source: ";
		if (EInputDataSource::ExternalFile == inputDataSource)
		{
			fileStream << "External File - ";
			std::string filename;
			DSC::DataManager::getUnitAttribute(EUnitId_Heater, "TestDataInputFilename", filename);
			fileStream << filename << std::endl;
			std::string description;
			DSC::DataManager::getUnitAttribute(EUnitId_Heater, "TestDataInputDescription", description);
			fileStream << description << std::endl;
			
		}
		else if (EInputDataSource::UniformDistribution == inputDataSource)
		{
			fileStream << "Internal Data Creator - Generated from Uniform Distribution (range 0 - 60)" << std::endl;
		}

		const auto samplingPeriod = static_cast<u64>(DSC::DataManager::getData(EDataType::ModelIdentificationSamplingPeriod));
		const auto fileDataCollectingPeriod = static_cast<u64>(1000.0 * DSC::DataManager::getData(EDataType::HeaterPowerControlFileDataSampling));
		const auto numberOfSamples = static_cast<u64>(DSC::DataManager::getData(EDataType::ModelIdentificationNumberOfSamples));
		const auto experimentTime = static_cast<u64>(DSC::DataManager::getData(EDataType::ModelIdentificationExperimentTime));
		const auto experimentTimeHours = static_cast<u16>(experimentTime / (1000 * 60 * 60));
		const auto experimentTimeMinutes = static_cast<u16>((experimentTime % (1000 * 60 * 60)) / (1000 * 60));
		const auto experimentTimeSeconds = static_cast<u16>(((experimentTime % (1000 * 60 * 60)) % (1000 * 60)) / 1000);
		fileStream << "Number of Samples: " << numberOfSamples << std::endl;
		fileStream << "CV Sampling Period: " << samplingPeriod << " ms" << std::endl;
		fileStream << "Data Collecting to File Period: " << fileDataCollectingPeriod << " ms" << std::endl;
		fileStream << "Duration of the Experiment: ";
		if (0UL == experimentTimeHours && 0UL == experimentTimeMinutes)
		{
			fileStream << experimentTimeSeconds << " sec.";
		}
		else if (0UL == experimentTimeHours)
		{
			fileStream << experimentTimeMinutes << " min. " << experimentTimeSeconds << " sec.";
		}
		else
		{
			fileStream << experimentTimeHours << " hrs " << experimentTimeMinutes << " min. " << experimentTimeSeconds << " sec.";
		}
		fileStream << std::endl << std::endl;
	}

    std::string FileDataManager::getActualTime()
    {
        auto actualTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        auto localTime = std::localtime(&actualTime);
		
        std::locale locale;
        const auto & tempPut = std::use_facet<std::time_put<char>>(locale);
        std::string timePattern;
        std::stringstream timeStream;

        timePattern = "%T";
        tempPut.put(timeStream, timeStream, ' ', localTime, timePattern.c_str(), timePattern.c_str() + timePattern.length());
        
        return timeStream.str();
    }

    std::string FileDataManager::getActualDate()
    {
        auto actualTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        auto localTime = std::localtime(&actualTime);

        std::locale locale;
        const auto & tempPut = std::use_facet<std::time_put<char>>(locale);
        std::string timePattern;
        std::stringstream timeStream;

        timePattern = "%F";
        tempPut.put(timeStream, timeStream, ' ', localTime, timePattern.c_str(), timePattern.c_str() + timePattern.length());

        return timeStream.str();
    }

    std::string FileDataManager::getActualWeekday()
    {
        auto actualTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        auto localTime = std::localtime(&actualTime);

        std::locale locale;
        const auto & tempPut = std::use_facet<std::time_put<char>>(locale);
        std::string timePattern;
        std::stringstream timeStream;

        timePattern = "%A";
        tempPut.put(timeStream, timeStream, ' ', localTime, timePattern.c_str(), timePattern.c_str() + timePattern.length());

        return timeStream.str();
    }

    std::string FileDataManager::convertDoubleToString(const double & value, int precision)
    {
        if (DSC::DataManager::UnknownValue == value)
        {
            return "N/A";
        }

        std::stringstream stream;
        stream << std::fixed << std::setprecision(precision) << value;
        return stream.str();
    }

    const std::string & FileDataManager::getLoggerPrefix()
    {
        static std::string loggerPrefix("FileDataManager");
        return loggerPrefix;
    }

    std::mutex FileDataManager::mMtx;
    std::ofstream FileDataManager::mHeaterDataFileStream;
    std::ofstream FileDataManager::mDscDataFileStream;
    boost::filesystem::path FileDataManager::mHeaterControlSystemDataDirectoryPath;
    boost::filesystem::path FileDataManager::mDscDataDirectoryPath;
    boost::filesystem::path FileDataManager::mHeaterControlSystemDataFilePath;
    boost::filesystem::path FileDataManager::mDscDataFilePath;
    unsigned long long FileDataManager::mHeaterDataSampleNumber;
    unsigned long long FileDataManager::mDscDataSampleNumber;
    DSC::DataManager::CallbackId FileDataManager::mNewAttributeCallbackId;
    bool FileDataManager::mIsNewAttributeCallbackRegistered;
    DSC::DataManager::CallbackId FileDataManager::mNewControlModeCallbackId;
    TimerManager::TimerId FileDataManager::mHeaterControlSystemStoreDataCallbackId;
    TimerManager::TimerId FileDataManager::mDscStoreDataCallbackId;
    EControlMode FileDataManager::mControlMode;
    bool FileDataManager::mIsHeaterPowerControlDataRegisteringActive;
    bool FileDataManager::mIsDscDataRegisteringActive;
}