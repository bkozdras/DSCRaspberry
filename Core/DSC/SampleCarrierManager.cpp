#include "SampleCarrierManager.hpp"
#include "../Utilities/Logger.hpp"
#include "../Utilities/ToStringConverter.hpp"
#include "DataManager.hpp"
#include "../Nucleo/DeviceCommunicator.hpp"

#include <map>
#include <cmath>

namespace DSC
{
    bool SampleCarrierDataManager::initialize()
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        DataManager::updateData(EDataType::SampleCarrierTemperature, DataManager::UnknownValue);
        DataManager::updateData(EDataType::ReferenceThermocouple, DataManager::UnknownValue);
        DataManager::updateData(EDataType::Thermocouple1, DataManager::UnknownValue);
        DataManager::updateData(EDataType::Thermocouple2, DataManager::UnknownValue);
        DataManager::updateData(EDataType::Thermocouple3, DataManager::UnknownValue);
        DataManager::updateData(EDataType::Thermocouple4, DataManager::UnknownValue);
        DataManager::updateData(EDataType::FilteredThermocouple1, DataManager::UnknownValue);
        DataManager::updateData(EDataType::FilteredThermocouple2, DataManager::UnknownValue);
        DataManager::updateData(EDataType::FilteredThermocouple3, DataManager::UnknownValue);
        DataManager::updateData(EDataType::FilteredThermocouple4, DataManager::UnknownValue);

        initializeFilterData();
        initializeAverageFilterData();

        {
            Nucleo::DeviceCommunicator::registerIndCallback
            (
                [](TSampleCarrierDataInd && ind)
                {
                    sampleCarrierDataIndCallback(std::move(ind));
                }
            );
        }

        Logger::info("%s: Initialized!", getLoggerPrefix().c_str());
        return true;
    }

    float SampleCarrierDataManager::getTemperature()
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        auto temperature = DataManager::getData(EDataType::SampleCarrierTemperature);
        return temperature;
    }

    float SampleCarrierDataManager::getThermocoupleValue(EUnitId thermocouple)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);

        if (!isUnitThermocouple(thermocouple))
        {
            Logger::error("%s: Getting thermocouple value: %s unit is not thermocouple!", getLoggerPrefix().c_str(), ToStringConverter::getUnitId(thermocouple).c_str());
            return DataManager::UnknownValue;
        }

        return DataManager::getData(convertThermocoupleUnitIdToDataType(thermocouple));
    }

    void SampleCarrierDataManager::updateFilterData()
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);

        mFilterData[0].first = DSC::DataManager::getData(EDataType::FilteringThreshold1);
        mFilterData[0].second = DSC::DataManager::getData(EDataType::FilteringThreshold1Coefficient);

        mFilterData[1].first = DSC::DataManager::getData(EDataType::FilteringThreshold2);
        mFilterData[1].second = DSC::DataManager::getData(EDataType::FilteringThreshold2Coefficient);

        mFilterData[2].first = DSC::DataManager::getData(EDataType::FilteringThreshold3);
        mFilterData[2].second = DSC::DataManager::getData(EDataType::FilteringThreshold3Coefficient);

        mFilterData[3].first = DSC::DataManager::getData(EDataType::FilteringThreshold4);
        mFilterData[3].second = DSC::DataManager::getData(EDataType::FilteringThreshold4Coefficient);

        Logger::info("%s: Updated filter thresholds and coefficients.", getLoggerPrefix().c_str());
    }

    void SampleCarrierDataManager::changeThresholdFilteringStatus(FilteringStatus status)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);

        if (FilteringStatus::Enabled == status)
        {
            initializeFilterData();
        }
        else
        {
            deinitializeFilterData();
        }
    }

    void SampleCarrierDataManager::updateAverageFilterData()
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);

        mNAverageCoefficient = DataManager::getData(EDataType::FilteringAverageCoefficientN);
        mXAverageCoefficient = DataManager::getData(EDataType::FilteringAverageCoefficientX);

        mFilterAverageVector[EDataType::Thermocouple1].first = 0U;
        mFilterAverageVector[EDataType::Thermocouple1].second.clear();

        mFilterAverageVector[EDataType::Thermocouple2].first = 0U;
        mFilterAverageVector[EDataType::Thermocouple2].second.clear();

        mFilterAverageVector[EDataType::Thermocouple3].first = 0U;
        mFilterAverageVector[EDataType::Thermocouple3].second.clear();

        mFilterAverageVector[EDataType::Thermocouple4].first = 0U;
        mFilterAverageVector[EDataType::Thermocouple4].second.clear();

        Logger::info("%s: Updated average filter coefficients (N = %u, X = %u).", getLoggerPrefix().c_str(), mNAverageCoefficient, mXAverageCoefficient);
    }

    void SampleCarrierDataManager::changeAverageFilteringStatus(FilteringStatus status)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);

        if (FilteringStatus::Enabled == status)
        {
            initializeAverageFilterData();
        }
        else
        {
            deinitializeAverageFilterData();
        }
    }

    void SampleCarrierDataManager::startRegisteringData()
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);

        Logger::debug("%s: Starting registering Sample Carrier data...", getLoggerPrefix().c_str());
        TStartRegisteringDataRequest request;
        request.dataType = ERegisteringDataType_SampleCarrierData;
        Nucleo::DeviceCommunicator::send(request, [](TStartRegisteringDataResponse && response, bool isNotTimeout){ startRegisteringDataResponseCallback(std::move(response)); });
    }

    void SampleCarrierDataManager::stopRegisteringData()
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);

        Logger::debug("%s: Stopping registering Sample Carrier data...", getLoggerPrefix().c_str());
        TStopRegisteringDataRequest request;
        request.dataType = ERegisteringDataType_SampleCarrierData;
        Nucleo::DeviceCommunicator::send(request, [](TStopRegisteringDataResponse && response, bool isNotTimeout){ stopRegisteringDataResponseCallback(std::move(response)); });
    }

    void SampleCarrierDataManager::initializeFilterData()
    {
        DataManager::updateData(EDataType::FilteringThreshold1, 50.0);
        DataManager::updateData(EDataType::FilteringThreshold1Coefficient, 0.5);
        mFilterData.push_back(std::make_pair(50.0, 0.5));

        DataManager::updateData(EDataType::FilteringThreshold2, 25.0);
        DataManager::updateData(EDataType::FilteringThreshold2Coefficient, 0.7);
        mFilterData.push_back(std::make_pair(25.0, 0.7));

        DataManager::updateData(EDataType::FilteringThreshold3, 10.0);
        DataManager::updateData(EDataType::FilteringThreshold3Coefficient, 0.8);
        mFilterData.push_back(std::make_pair(10.0, 0.8));
        
        DataManager::updateData(EDataType::FilteringThreshold4, 1.0);
        DataManager::updateData(EDataType::FilteringThreshold4Coefficient, 0.0);
        mFilterData.push_back(std::make_pair(1.0, 0.0));

        mIsThresholdFilteringEnabled = true;

        Logger::info("%s: Threshold filtering DSC data enabled.", getLoggerPrefix().c_str());
    }

    void SampleCarrierDataManager::initializeAverageFilterData()
    {
        DataManager::updateData(EDataType::FilteringAverageCoefficientN, 20.0);
        DataManager::updateData(EDataType::FilteringAverageCoefficientX, 5.0);
        mNAverageCoefficient = 20.0;
        mXAverageCoefficient = 5.0;
        mFilterAverageVector[EDataType::Thermocouple1] = std::make_pair(0U, std::vector<double>());
        mFilterAverageVector[EDataType::Thermocouple2] = std::make_pair(0U, std::vector<double>());
        mFilterAverageVector[EDataType::Thermocouple3] = std::make_pair(0U, std::vector<double>());
        mFilterAverageVector[EDataType::Thermocouple4] = std::make_pair(0U, std::vector<double>());
        mIsAverageFilteringEnabled = true;

        Logger::info("%s: Average filtering DSC data enabled.", getLoggerPrefix().c_str());
    }

    void SampleCarrierDataManager::deinitializeFilterData()
    {
        mFilterData.clear();
        mIsThresholdFilteringEnabled = false;

        Logger::info("%s: Threshold filtering DSC data disabled.", getLoggerPrefix().c_str());
    }

    void SampleCarrierDataManager::deinitializeAverageFilterData()
    {
        mFilterAverageVector.clear();
        mIsAverageFilteringEnabled = false;

        Logger::info("%s: Average filtering DSC data disabled.", getLoggerPrefix().c_str());
    }

    void SampleCarrierDataManager::sampleCarrierDataIndCallback(TSampleCarrierDataInd && ind)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);

        Logger::debug("%s: Received SampleCarrierDataInd.", getLoggerPrefix().c_str());

        switch (ind.data.unitId)
        {
            case EUnitId_Rtd1Pt100:
            {
                Logger::debug("%s: Sample carrier temperature: %.2f oC.", getLoggerPrefix().c_str(), ind.data.value);
                DataManager::updateData(EDataType::SampleCarrierTemperature, ind.data.value);
                break;
            }

            case EUnitId_ThermocoupleReference :
            {
                Logger::debug("%s: Sample carrier %s value: %.4f uV.", getLoggerPrefix().c_str(), ToStringConverter::getUnitId(EUnitId_ThermocoupleReference).c_str(), ind.data.value);
                DataManager::updateData(convertThermocoupleUnitIdToDataType(EUnitId_ThermocoupleReference), ind.data.value);
                break;
            }

            case EUnitId_Thermocouple1 :
            {
                Logger::debug("%s: Sample carrier %s value: %.4f mV.", getLoggerPrefix().c_str(), ToStringConverter::getUnitId(EUnitId_Thermocouple1).c_str(), ind.data.value);
                filterDataAndUpdate(EDataType::Thermocouple1, ind.data.value);
                //DataManager::updateData(convertThermocoupleUnitIdToDataType(EUnitId_Thermocouple1), ind.data.value);
                break;
            }

            case EUnitId_Thermocouple2 :
            {
                Logger::debug("%s: Sample carrier %s value: %.4f mV.", getLoggerPrefix().c_str(), ToStringConverter::getUnitId(EUnitId_Thermocouple2).c_str(), ind.data.value);
                filterDataAndUpdate(EDataType::Thermocouple2, ind.data.value);
                //DataManager::updateData(convertThermocoupleUnitIdToDataType(EUnitId_Thermocouple2), ind.data.value);
                break;
            }

            case EUnitId_Thermocouple3 :
            {
                Logger::debug("%s: Sample carrier %s value: %.4f mV.", getLoggerPrefix().c_str(), ToStringConverter::getUnitId(EUnitId_Thermocouple3).c_str(), ind.data.value);
                filterDataAndUpdate(EDataType::Thermocouple3, ind.data.value);
                //DataManager::updateData(convertThermocoupleUnitIdToDataType(EUnitId_Thermocouple3), ind.data.value);
                break;
            }

            case EUnitId_Thermocouple4 :
            {
                Logger::debug("%s: Sample carrier %s value: %.4f mV.", getLoggerPrefix().c_str(), ToStringConverter::getUnitId(EUnitId_Thermocouple4).c_str(), ind.data.value);
                filterDataAndUpdate(EDataType::Thermocouple4, ind.data.value);
                //DataManager::updateData(convertThermocoupleUnitIdToDataType(EUnitId_Thermocouple4), ind.data.value);
                break;
            }

            default :
                break;
        }
    }

    void SampleCarrierDataManager::startRegisteringDataResponseCallback(TStartRegisteringDataResponse && response)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);

        if (ERegisteringDataType_SampleCarrierData == response.dataType)
        {
            if (response.success)
            {
                Logger::info("%s: Started registering Sample Carrier data values.", getLoggerPrefix().c_str());
                DataManager::updateUnitAttribute(EUnitId_ADS1248, "DataRegistering", "Enabled");
            }
            else
            {
                Logger::error("%s: Starting registering Sample Carrier data values failed!", getLoggerPrefix().c_str());
            }
        }
        else
        {
            Logger::error("%s: Wrong response callback caught in SampleCarrierDataManager!", getLoggerPrefix().c_str());
        }
    }

    void SampleCarrierDataManager::stopRegisteringDataResponseCallback(TStopRegisteringDataResponse && response)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);

        if (ERegisteringDataType_SampleCarrierData == response.dataType)
        {
            if (response.success)
            {
                Logger::info("%s: Stopped registering Sample Carrier data values.", getLoggerPrefix().c_str());
                DataManager::updateUnitAttribute(EUnitId_LMP90100ControlSystem, "DataRegistering", "Disabled");
            }
            else
            {
                Logger::error("%s: Stopping registering Sample Carrier data values failed!", getLoggerPrefix().c_str());
            }
        }
        else
        {
            Logger::error("%s: Wrong response callback caught in SampleCarrierDataManager!", getLoggerPrefix().c_str());
        }
    }

    const std::string & SampleCarrierDataManager::getLoggerPrefix()
    {
        static std::string loggerPrefix("SampleCarrierDataManager");
        return loggerPrefix;
    }

    bool SampleCarrierDataManager::isUnitThermocouple(EUnitId unitId)
    {
        switch (unitId)
        {
            case EUnitId_Thermocouple1 :
            case EUnitId_Thermocouple2:
            case EUnitId_Thermocouple3:
            case EUnitId_Thermocouple4:
            case EUnitId_ThermocoupleReference:
            {
                return true;
            }

            default:
            {
                return false;
            }
        }
    }

    EDataType SampleCarrierDataManager::convertThermocoupleUnitIdToDataType(EUnitId unitId)
    {
        static std::map<EUnitId, EDataType> thermocoupleToDataType = decltype(thermocoupleToDataType)
        {
            std::make_pair(EUnitId_Thermocouple1, EDataType::Thermocouple1),
            std::make_pair(EUnitId_Thermocouple2, EDataType::Thermocouple2),
            std::make_pair(EUnitId_Thermocouple3, EDataType::Thermocouple3),
            std::make_pair(EUnitId_Thermocouple4, EDataType::Thermocouple4),
            std::make_pair(EUnitId_ThermocoupleReference, EDataType::ReferenceThermocouple)
        };

        return thermocoupleToDataType[unitId];
    }

    EDataType SampleCarrierDataManager::getFilteredDataType(EDataType type)
    {
        static std::map<EDataType, EDataType> dataToFiltered = decltype(dataToFiltered)
        {
            std::make_pair(EDataType::Thermocouple1, EDataType::FilteredThermocouple1),
            std::make_pair(EDataType::Thermocouple2, EDataType::FilteredThermocouple2),
            std::make_pair(EDataType::Thermocouple3, EDataType::FilteredThermocouple3),
            std::make_pair(EDataType::Thermocouple4, EDataType::FilteredThermocouple4)
        };

        return dataToFiltered[type];
    }

    void SampleCarrierDataManager::filterDataAndUpdate(EDataType dataType, const double & newValue)
    {
        static auto isFirstEntering = true;
        static u8 counter = 0;

        auto actualFilteredValue = DSC::DataManager::getData(getFilteredDataType(dataType));
        if (isFirstEntering && (counter++ < 4))
        {
            actualFilteredValue = newValue;
        }
        else
        {
            isFirstEntering = false;
        }
        
        auto averageFilteringOutput = newValue;
        DataManager::updateData(dataType, newValue);
        auto isThresholdFilteringAllowed = false;

        if (mIsAverageFilteringEnabled)
        {
            auto & filterData = mFilterAverageVector[dataType];
            auto & xInBufferCount = filterData.first;
            auto & buffer = filterData.second;

            buffer.push_back(newValue);
            ++xInBufferCount;

            if ((buffer.size() + 1) >= mNAverageCoefficient)
            {
                buffer.erase(std::begin(buffer));

                if (xInBufferCount >= mXAverageCoefficient)
                {
                    const auto nSubX = mNAverageCoefficient - mXAverageCoefficient;
                    double firstAverageValue = 0.0;
                    double secondAverageValue = 0.0;

                    {
                        auto iter = std::begin(buffer);
                        std::advance(iter, nSubX);
                        firstAverageValue = std::accumulate(std::begin(buffer), iter, 0.0) / nSubX;
                    }

                    {
                        auto iter = std::begin(buffer);
                        std::advance(iter, nSubX + 1);
                        secondAverageValue = std::accumulate(iter, std::end(buffer), 0.0) / mXAverageCoefficient;
                    }

                    averageFilteringOutput = (nSubX * firstAverageValue + mXAverageCoefficient * secondAverageValue) / mNAverageCoefficient;

                    xInBufferCount = 0;
                    isThresholdFilteringAllowed = true;
                }
            }
        }

        if (mIsThresholdFilteringEnabled && isThresholdFilteringAllowed)
        {
            const auto difference = std::abs(averageFilteringOutput - actualFilteredValue);
            auto filtered = false;

            auto filterData =
                [&filtered, &actualFilteredValue, &averageFilteringOutput, &difference](const double & threshold, const double & coefficient)
            {
                if (!filtered && difference > threshold)
                {
                    actualFilteredValue = (actualFilteredValue * coefficient) + (averageFilteringOutput * (1.0 - coefficient));
                    filtered = true;
                }
            };

            filterData(mFilterData[0].first, mFilterData[0].second);
            filterData(mFilterData[1].first, mFilterData[1].second);
            filterData(mFilterData[2].first, mFilterData[2].second);
            filterData(-0.1, mFilterData[3].second);
        }

        if (mIsThresholdFilteringEnabled)
        {
            DataManager::updateData(getFilteredDataType(dataType), actualFilteredValue);
        }
        else if (isThresholdFilteringAllowed)
        {
            DataManager::updateData(getFilteredDataType(dataType), averageFilteringOutput);
        }
        else
        {
            DataManager::updateData(getFilteredDataType(dataType), actualFilteredValue);
        }
    }

    std::mutex SampleCarrierDataManager::mMtx;
    std::vector<std::pair<double, double>> SampleCarrierDataManager::mFilterData;
    std::map<EDataType, std::pair<unsigned int, std::vector<double>>> SampleCarrierDataManager::mFilterAverageVector;
    unsigned int SampleCarrierDataManager::mNAverageCoefficient;
    unsigned int SampleCarrierDataManager::mXAverageCoefficient;
    bool SampleCarrierDataManager::mIsThresholdFilteringEnabled;
    bool SampleCarrierDataManager::mIsAverageFilteringEnabled;
}