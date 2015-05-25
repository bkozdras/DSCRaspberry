#include "FileDataManager.hpp"

#include <boost/tokenizer.hpp>

#include "../Utilities/Logger.hpp"
#include "../Utilities/ToStringConverter.hpp"
#include "../FaultManagement/FaultManager.hpp"

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

            saveToFileCurrentData(mHeaterDataFileStream);
            saveToFileCurrentControlMode(mHeaterDataFileStream);
            saveToFileRegisteringPeriod(mHeaterDataFileStream);
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
        }
        else
        {
            Logger::error("%s: Starting registering heater control system data failed...", getLoggerPrefix().c_str());
        }
    }

    void FileDataManager::_stopRegisteringHeaterControlSystemData()
    {
        if (closeFile(mHeaterDataFileStream))
        {
            TimerManager::destroy(mHeaterControlSystemStoreDataCallbackId);
            DSC::DataManager::deregisterNewControlModeCallback(mNewControlModeCallbackId);

            Logger::info("%s: Stopped registering heater control system data.", getLoggerPrefix().c_str());
            Logger::info("%s: Saved file: %s.", getLoggerPrefix().c_str(), mHeaterControlSystemDataFilePath.c_str());
        }
    }

    void FileDataManager::_startRegisteringDscData()
    {
    }

    void FileDataManager::_stopRegisteringDscData()
    {
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
                auto cvValue = DSC::DataManager::getData(EDataType::HeaterPower);
                auto pvValue = DSC::DataManager::getData(EDataType::HeaterTemperature);

                mHeaterDataFileStream << convertDoubleToString(cvValue, 2);
                mHeaterDataFileStream << "\t";
                mHeaterDataFileStream << convertDoubleToString(pvValue, 2);

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

    void FileDataManager::saveToFileRegisteringPeriod(std::ofstream & fileStream)
    {
        double registeringPeriod;

        if (mHeaterDataFileStream == fileStream)
        {
            registeringPeriod = DSC::DataManager::getData(EDataType::HeaterPowerControlFileDataSampling);
        }
        else if (mDscDataFileStream == fileStream)
        {
            return;
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
                fileStream << "Power" << "\t" << "Temperature";
            }
            else
            {
                fileStream << "SP" << "\t" << "CV" << "\t" << "PV" << "\t" << "ERR";
            }
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
}