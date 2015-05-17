#include "SMPCBTemperatureManager.hpp"
#include "../Utilities/Logger.hpp"
#include "../Utilities/ToStringConverter.hpp"
#include "DataManager.hpp"
#include "../Nucleo/DeviceCommunicator.hpp"

namespace DSC
{
    bool SMPCBTemperatureManager::initialize()
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        DataManager::updateData(EDataType::SMPCBTemperature, DataManager::UnknownValue);

        {
            Nucleo::DeviceCommunicator::registerIndCallback
            (
                [](TReferenceTemperatureInd && ind)
                {
                    referenceTemperatureIndCallback(std::move(ind));
                }
            );
        }

        Logger::info("%s: Initialized!", getLoggerPrefix().c_str());
        return true;
    }

    bool SMPCBTemperatureManager::startControlling()
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);

        TStartReferenceTemperatureStabilizationRequest request;
        Nucleo::DeviceCommunicator::send(request, [](TStartReferenceTemperatureStabilizationResponse && response, bool isNotTimeout){ startReferenceTemperatureStabilizationResponseCallback(std::move(response)); });

        return true;
    }

    bool SMPCBTemperatureManager::stopControlling()
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);

        TStopReferenceTemperatureStabilizationRequest request;
        Nucleo::DeviceCommunicator::send(request, [](TStopReferenceTemperatureStabilizationResponse && response, bool isNotTimeout){ stopReferenceTemperatureStabilizationResponseCallback(std::move(response)); });

        return true;
    }

    float SMPCBTemperatureManager::getTemperature()
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        auto temperature = DataManager::getData(EDataType::SMPCBTemperature);
        return temperature;
    }

    void SMPCBTemperatureManager::startRegisteringTemperatureValue()
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);

        Logger::debug("%s: Starting registering Signals Measurement PCB temperature value...", getLoggerPrefix().c_str());
        TStartRegisteringDataRequest request;
        request.dataType = ERegisteringDataType_ReferenceTemperature;
        Nucleo::DeviceCommunicator::send(request, [](TStartRegisteringDataResponse && response, bool isNotTimeout){ startRegisteringDataResponseCallback(std::move(response)); });
    }

    void SMPCBTemperatureManager::stopRegisteringTemperatureValue()
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        Logger::debug("%s: Stopping registering Signals Measurement PCB temperature value...", getLoggerPrefix().c_str());
        TStopRegisteringDataRequest request;
        request.dataType = ERegisteringDataType_ReferenceTemperature;
        Nucleo::DeviceCommunicator::send(request, [](TStopRegisteringDataResponse && response, bool isNotTimeout){ stopRegisteringDataResponseCallback(std::move(response)); });
    }

    void SMPCBTemperatureManager::referenceTemperatureIndCallback(TReferenceTemperatureInd && ind)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        Logger::debug("%s: Received ReferenceTemperatureInd. New value: %.2f oC.", getLoggerPrefix().c_str(), ind.temperature);
        DataManager::updateData(EDataType::SMPCBTemperature, ind.temperature);
    }

    void SMPCBTemperatureManager::startReferenceTemperatureStabilizationResponseCallback(TStartReferenceTemperatureStabilizationResponse && response)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);

        if (response.success)
        {
            Logger::info("%s: Started controlling SM PCB temperature value.", getLoggerPrefix().c_str());
        }
        else
        {
            Logger::error("%s: Starting controlling SM PCB temperature value failed!", getLoggerPrefix().c_str());
        }
    }

    void SMPCBTemperatureManager::stopReferenceTemperatureStabilizationResponseCallback(TStopReferenceTemperatureStabilizationResponse && response)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);

        if (response.success)
        {
            Logger::info("%s: Stopped controlling SM PCB temperature value.", getLoggerPrefix().c_str());
        }
        else
        {
            Logger::error("%s: Stopping controlling SM PCB temperature value failed!", getLoggerPrefix().c_str());
        }
    }

    void SMPCBTemperatureManager::startRegisteringDataResponseCallback(TStartRegisteringDataResponse && response)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);

        if (ERegisteringDataType_ReferenceTemperature == response.dataType)
        {
            if (response.success)
            {
                Logger::info("%s: Started registering Signals Measurement PCB temperature value.", getLoggerPrefix().c_str());
            }
            else
            {
                Logger::error("%s: Starting registering Signals Measurement PCB temperature value failed!", getLoggerPrefix().c_str());
            }
        }
        else
        {
            Logger::error("%s: Wrong response callback caught in SMPCBTemperatureManager!", getLoggerPrefix().c_str());
        }
    }

    void SMPCBTemperatureManager::stopRegisteringDataResponseCallback(TStopRegisteringDataResponse && response)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);

        if (ERegisteringDataType_ReferenceTemperature == response.dataType)
        {
            if (response.success)
            {
                Logger::info("%s: Stopped registering Signals Measurement PCB temperature value.", getLoggerPrefix().c_str());
            }
            else
            {
                Logger::error("%s: Stopping registering Signals Measurement PCB temperature value failed!", getLoggerPrefix().c_str());
            }
        }
        else
        {
            Logger::error("%s: Wrong response callback caught in SMPCBTemperatureManager!", getLoggerPrefix().c_str());
        }
    }

    const std::string & SMPCBTemperatureManager::getLoggerPrefix()
    {
        static std::string loggerPrefix("SMPCBTemperatureManager");
        return loggerPrefix;
    }

    std::mutex SMPCBTemperatureManager::mMtx;
}