#include "SampleCarrierManager.hpp"
#include "../Utilities/Logger.hpp"
#include "../Utilities/ToStringConverter.hpp"
#include "DataManager.hpp"
#include "../Nucleo/DeviceCommunicator.hpp"

#include <map>

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

    void SampleCarrierDataManager::sampleCarrierDataIndCallback(TSampleCarrierDataInd && ind)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);

        Logger::debug("%s: Received SampleCarrierDataInd.", getLoggerPrefix().c_str());
        Logger::debug("%s: Sample carrier temperature: %.2f oC.", getLoggerPrefix().c_str(), ind.data.rtdTemperatureValue);
        DataManager::updateData(EDataType::SampleCarrierTemperature, ind.data.rtdTemperatureValue);

        for (u8 iter = 0; THERMOCOUPLES_COUNT > iter; ++iter)
        {
            Logger::debug("%s: Sample carrier %s value: %.4 mV.", getLoggerPrefix().c_str(), ToStringConverter::getUnitId(ind.data.data[iter].thermocouple).c_str(), ind.data.data[iter].milliVoltVoltage);
            DataManager::updateData(convertThermocoupleUnitIdToDataType(ind.data.data[iter].thermocouple), ind.data.data[iter].milliVoltVoltage);
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

    std::mutex SampleCarrierDataManager::mMtx;
}