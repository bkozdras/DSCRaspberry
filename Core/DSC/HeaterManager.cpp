#include "HeaterManager.hpp"
#include "../Utilities/Logger.hpp"
#include "../Utilities/ToStringConverter.hpp"
#include "DataManager.hpp"
#include "../Nucleo/DeviceCommunicator.hpp"
#include "../DevicePeripherals/UnitsDetector.hpp"
#include "../System/ThreadPool.hpp"

namespace DSC
{
    bool HeaterManager::initialize()
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);

        mNewControlModeSetCallback = decltype(mNewControlModeSetCallback)();

        DataManager::updateData(EDataType::HeaterPower, 0.0);
        DataManager::updateData(EDataType::HeaterTemperature, DataManager::UnknownValue);
        DataManager::updateData(EDataType::SPHeaterTemperature, DataManager::UnknownValue);
        DataManager::updateControlMode(EControlMode::NotSet);

        {
            Nucleo::DeviceCommunicator::registerIndCallback
            (
                [](THeaterTemperatureInd && ind)
                {
                    heaterTemperatureIndCallback(std::move(ind));
                }
            );

            Nucleo::DeviceCommunicator::registerIndCallback
            (
                [](TControllerDataInd && ind)
                {
                    controllerDataIndCallback(std::move(ind));
                }
            );
        }

        Logger::info("%s: Initialized!", getLoggerPrefix().c_str());
        return true;
    }

    bool HeaterManager::setPowerInPercent(float percent, std::function<void(float, bool)> newPowerValueCallback)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);

        if (EControlMode::OpenLoop != DataManager::getControlMode())
        {
            Logger::warning("%s: Setting power %.2f%% not allowed. Control mode is not set to Open Loop!", getLoggerPrefix().c_str(), percent);
            return false;
        }

        if (DSC::DataManager::getData(EDataType::HeaterPower) != percent)
        {
            mNewPowerValueSetCallback = newPowerValueCallback;

            TSetHeaterPowerRequest request;
            request.power = convertPercentPowerToU16(percent);

            Logger::info("%s: Setting heater power value to %.2f%% (%u).", getLoggerPrefix().c_str(), percent, request.power);

            Nucleo::DeviceCommunicator::send(request, [](TSetHeaterPowerResponse && response, bool isNotTimeout){ heaterPowerResponseCallback(std::move(response)); });
        }

        return true;
    }

    bool HeaterManager::setPower(u16 power)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        if (EControlMode::OpenLoop != DataManager::getControlMode())
        {
            Logger::warning("%s: Setting power %u not allowed. Control mode is not set to Open Loop!", getLoggerPrefix().c_str(), power);
            return false;
        }

        if (1023U < power)
        {
            power = 1023U;
        }

        TSetHeaterPowerRequest request;
        request.power = power;
        Nucleo::DeviceCommunicator::send(request, [](TSetHeaterPowerResponse && response, bool isNotTimeout){ heaterPowerResponseCallback(std::move(response)); });

        return true;
    }

    float HeaterManager::getPowerInPercent()
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        auto power = DataManager::getData(EDataType::HeaterPower);
        return power;
    }

    u16 HeaterManager::getPower()
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        auto power = DataManager::getData(EDataType::HeaterPower);
        return convertPercentPowerToU16(power);
    }

    float HeaterManager::getTemperature()
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        auto temperature = DataManager::getData(EDataType::HeaterTemperature);
        return temperature;
    }

    bool HeaterManager::setPowerControlMode(EControlMode mode, std::function<void(EControlMode, bool)> newModeSetCallback)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);

        auto controlType = convertEControlModeToEControlSystemType(mode);

        if (DataManager::getControlMode() == mode)
        {
            Logger::info("%s: Setting new power control mode %s skipped. New mode is the same as set previously...", getLoggerPrefix().c_str(), ToStringConverter::getControlSystemType(controlType).c_str());
            return true;
        }

        mNewControlModeSetCallback = newModeSetCallback;
        TSetControlSystemTypeRequest request;
        request.type = controlType;
        Nucleo::DeviceCommunicator::send(request, [](TSetControlSystemTypeResponse && response, bool isNotTimeout){ setControlSystemTypeResponse(std::move(response)); });

        return true;
    }

    EControlMode HeaterManager::getPowerControlMode()
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        return DataManager::getControlMode();
    }

    bool HeaterManager::setTemperatureInFeedbackMode(float value)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);

        const auto controlMode = DataManager::getControlMode();

        if (controlMode == EControlMode::NotSet || controlMode == EControlMode::OpenLoop)
        {
            Logger::warning("%s: Setting temperature in Feedback mode not allowed in actual control mode.", getLoggerPrefix().c_str());
            return false;
        }

        TSetHeaterTemperatureInFeedbackModeRequest request;
        request.temperature = value;
        Nucleo::DeviceCommunicator::send(request, [](TSetHeaterTemperatureInFeedbackModeResponse && response, bool isNotTimeout){ setHeaterTemperatureInFeedbackModeResponse(std::move(response)); });

        return true;
    }

    bool HeaterManager::setControllingAlgorithmExecutionPeriod(u16 value)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);

        TSetControllingAlgorithmExecutionPeriodRequest request;
        request.value = value;
        Nucleo::DeviceCommunicator::send(request, [](TSetControllingAlgorithmExecutionPeriodResponse && response, bool isNotTimeout){ setControllingAlgorithmExecutionPeriodResponse(std::move(response)); });

        return true;
    }

    void HeaterManager::startRegisteringTemperatureValue()
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);

        auto lmpMode = static_cast<ELMP90100Mode>(DataManager::getICMode(EUnitId_LMP90100ControlSystem));
        if (ELMP90100Mode_Off != lmpMode)
        {
            Logger::debug("%s: Starting registering heater temperature value...", getLoggerPrefix().c_str());
            TStartRegisteringDataRequest request;
            request.dataType = ERegisteringDataType_HeaterTemperature;
            Nucleo::DeviceCommunicator::send(request, [](TStartRegisteringDataResponse && response, bool isNotTimeout){ startRegisteringDataResponseCallback(std::move(response)); });
        }
        else
        {
            Logger::warning("%s: Starting registering heater temperature value failed! LMP90100 CS mode is Off!", getLoggerPrefix().c_str());
        }
    }

    void HeaterManager::stopRegisteringTemperatureValue()
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        Logger::debug("%s: Stopping registering heater temperature value...", getLoggerPrefix().c_str());
        TStopRegisteringDataRequest request;
        request.dataType = ERegisteringDataType_HeaterTemperature;
        Nucleo::DeviceCommunicator::send(request, [](TStopRegisteringDataResponse && response, bool isNotTimeout){ stopRegisteringDataResponseCallback(std::move(response)); });
    }

    void HeaterManager::startRegisteringControllerValues(u16 period)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);

        Logger::info("%s: Starting registering controller values in feedback mode (Period: %u ms).", getLoggerPrefix().c_str(), period);
        TStartRegisteringDataRequest request;
        request.dataType = ERegisteringDataType_ControllerData;
        request.period = period;
        Nucleo::DeviceCommunicator::send(request, [](TStartRegisteringDataResponse && response, bool isNotTimeout){ startRegisteringDataResponseCallback(std::move(response)); });
    }

    void HeaterManager::stopRegisteringControllerValues()
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        Logger::debug("%s: Stopping registering controller values in feedback mode.", getLoggerPrefix().c_str());
        TStopRegisteringDataRequest request;
        request.dataType = ERegisteringDataType_ControllerData;
        Nucleo::DeviceCommunicator::send(request, [](TStopRegisteringDataResponse && response, bool isNotTimeout){ stopRegisteringDataResponseCallback(std::move(response)); });
    }

    void HeaterManager::heaterTemperatureIndCallback(THeaterTemperatureInd && ind)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        Logger::debug("%s: Received HeaterTemperatureInd. New value: %.2f oC.", getLoggerPrefix().c_str(), ind.temperature);
        DataManager::updateData(EDataType::HeaterTemperature, ind.temperature);
    }

    void HeaterManager::controllerDataIndCallback(TControllerDataInd && ind)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        
        Logger::warning("CONTROLLER DATA IND: %u, %.2f.", ind.type, ind.value);

        switch (ind.type)
        {
            case EControllerDataType_CV:
            {
                DataManager::updateData(EDataType::CVHeaterTemperature, ind.value);
                break;
            }

            case EControllerDataType_ERR:
            {
                DataManager::updateData(EDataType::ERRHeaterTemperature, ind.value);
                break;
            }

            case EControllerDataType_PV:
            {
                DataManager::updateData(EDataType::HeaterTemperature, ind.value);
                break;
            }

            case EControllerDataType_SP:
            {
                DataManager::updateData(EDataType::SPHeaterTemperature, ind.value);
                break;
            }

            default :
                break;
        }
    }

    void HeaterManager::heaterPowerResponseCallback(TSetHeaterPowerResponse && response)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);

        auto powerPercent = convertU16ToPercentPower(response.power);

        if (response.success)
        {
            Logger::info("%s: Heater power %.2f%% set.", getLoggerPrefix().c_str(), powerPercent);
            DataManager::updateData(EDataType::HeaterPower, powerPercent);
        }
        else
        {
            Logger::error("%s: Setting heater power %.2f%% failed!", getLoggerPrefix().c_str(), powerPercent);
        }

        if (mNewPowerValueSetCallback)
        {
            auto callback = mNewPowerValueSetCallback;
            mNewPowerValueSetCallback = decltype(mNewPowerValueSetCallback)();
            ThreadPool::submit
            (
                TaskPriority::Normal,
                [response, callback]()
                {
                    std::lock_guard<std::mutex> lockGuard(mMtx);
                    callback(convertU16ToPercentPower(response.power), response.success);
                }
            );
        }
    }

    void HeaterManager::startRegisteringDataResponseCallback(TStartRegisteringDataResponse && response)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        
        if (ERegisteringDataType_HeaterTemperature == response.dataType)
        {
            if (response.success)
            {
                Logger::info("%s: Started registering heater temperature value.", getLoggerPrefix().c_str());
                DataManager::updateUnitAttribute(EUnitId_LMP90100ControlSystem, "DataRegistering", "Enabled");
            }
            else
            {
                Logger::error("%s: Starting registering heater temperature value failed!", getLoggerPrefix().c_str());
            }
        }
        else if (ERegisteringDataType_ControllerData == response.dataType)
        {
            if (response.success)
            {
                Logger::info("%s: Started registering controller values in feedback mode.", getLoggerPrefix().c_str());
                DataManager::updateUnitAttribute(EUnitId_HeaterTemperatureController, "DataRegistering", "Enabled");
            }
            else
            {
                Logger::error("%s: Starting registering controller values in feedback mode failed!", getLoggerPrefix().c_str());
            }
        }
        else
        {
            Logger::error("%s: Wrong response callback caught in HeaterManager!", getLoggerPrefix().c_str());
        }
    }

    void HeaterManager::stopRegisteringDataResponseCallback(TStopRegisteringDataResponse && response)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);

        if (ERegisteringDataType_HeaterTemperature == response.dataType)
        {
            if (response.success)
            {
                Logger::info("%s: Stopped registering heater temperature value.", getLoggerPrefix().c_str());
                DataManager::updateData(EDataType::HeaterTemperature, DataManager::UnknownValue);
                DataManager::updateUnitAttribute(EUnitId_LMP90100ControlSystem, "DataRegistering", "Disabled");
            }
            else
            {
                Logger::error("%s: Stopping registering heater temperature value failed!", getLoggerPrefix().c_str());
            }
        }
        else
        {
            Logger::error("%s: Wrong response callback caught in HeaterManager!", getLoggerPrefix().c_str());
        }
    }

    void HeaterManager::setControlSystemTypeResponse(TSetControlSystemTypeResponse && response)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);

        if (response.success)
        {
            Logger::info("%s: Changed heater power control system type to %s.", getLoggerPrefix().c_str(), ToStringConverter::getControlSystemType(response.type).c_str());
            DataManager::updateControlMode(convertEControlSystemTypeToEControlMode(response.type));
            DevicePeripherals::UnitsDetector::updateStatus(EUnitId_MCP4716, DevicePeripherals::UnitsDetector::Status::Working);
        }
        else
        {
            Logger::error("%s: Changing heater power control system type to %s failed!", getLoggerPrefix().c_str(), ToStringConverter::getControlSystemType(response.type).c_str());
        }

        if (mNewControlModeSetCallback)
        {
            auto callback = mNewControlModeSetCallback;
            mNewControlModeSetCallback = decltype(mNewControlModeSetCallback)();
            ThreadPool::submit
            (
                TaskPriority::Normal,
                [response, callback]()
                {
                    callback(convertEControlSystemTypeToEControlMode(response.type), response.success);
                }
            );
        }
    }

    void HeaterManager::setHeaterTemperatureInFeedbackModeResponse(TSetHeaterTemperatureInFeedbackModeResponse && response)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);

        if (response.success)
        {
            Logger::info("%s: Set heater temperature value in feedback mode: %.2f oC.", getLoggerPrefix().c_str(), response.temperature);
            DataManager::updateData(EDataType::SPHeaterTemperature, response.temperature);
        }
        else
        {
            Logger::error("%s: Setting heater temperature value in feedback mode: %.2f oC.", getLoggerPrefix().c_str(), response.temperature);
        }
    }

    void HeaterManager::setControllingAlgorithmExecutionPeriodResponse(TSetControllingAlgorithmExecutionPeriodResponse && response)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);

        if (response.success)
        {
            Logger::info("%s: Set heater temperature controlling algorithm execution period: %u ms.", getLoggerPrefix().c_str(), response.value);
            auto stringValue = std::to_string(response.value);
            DataManager::updateUnitAttribute(EUnitId_HeaterTemperatureController, "AlgorithmExecutionPeriod", stringValue);
        }
        else
        {
            Logger::error("%s: Setting heater temperature controlling algorithm execution period: %u ms.", getLoggerPrefix().c_str(), response.value);
        }
    }

    const std::string & HeaterManager::getLoggerPrefix()
    {
        static std::string loggerPrefix("HeaterManager");
        return loggerPrefix;
    }

    u16 HeaterManager::convertPercentPowerToU16(float power)
    {
        const auto maxValue = 100.00F;

        if (maxValue< power)
        {
            power = maxValue;
        }

        return static_cast<u16>( power * 10.0F );
    }

    float HeaterManager::convertU16ToPercentPower(u16 value)
    {
        auto floatValue = static_cast<float>(value);
        return ( floatValue / 10.0F );
    }

    EControlMode HeaterManager::convertEControlSystemTypeToEControlMode(EControlSystemType type)
    {
        static std::map<EControlSystemType, EControlMode> typeToMode = decltype(typeToMode)
        {
            std::make_pair(EControlSystemType_OpenLoop, EControlMode::OpenLoop),
            std::make_pair(EControlSystemType_SimpleFeedback, EControlMode::SimpleFeedback),
            std::make_pair(EControlSystemType_MFCFeedback, EControlMode::MFCFeedback)
        };

        return typeToMode[type];
    }

    EControlSystemType HeaterManager::convertEControlModeToEControlSystemType(EControlMode mode)
    {
        static std::map<EControlMode, EControlSystemType> modeToType = decltype(modeToType)
        {
            std::make_pair(EControlMode::OpenLoop, EControlSystemType_OpenLoop),
            std::make_pair(EControlMode::SimpleFeedback, EControlSystemType_SimpleFeedback),
            std::make_pair(EControlMode::MFCFeedback, EControlSystemType_MFCFeedback)
        };

        return modeToType[mode];
    }

    std::mutex HeaterManager::mMtx;
    std::function<void(EControlMode, bool)> HeaterManager::mNewControlModeSetCallback;
    std::function<void(float, bool)> HeaterManager::mNewPowerValueSetCallback;
}