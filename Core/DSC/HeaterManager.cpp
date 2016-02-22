#include "HeaterManager.hpp"
#include "../Utilities/Logger.hpp"
#include "../Utilities/ToStringConverter.hpp"
#include "DataManager.hpp"
#include "../Nucleo/DeviceCommunicator.hpp"
#include "../DevicePeripherals/UnitsDetector.hpp"
#include "../System/ThreadPool.hpp"
#include "../System/TimerManager.hpp"
#include "../ModelIdentification/ExperimentManager.hpp"
#include "../FaultManagement/FaultManager.hpp"
#include "../SharedDefines/EFaultId.h"
#include "../ModelIdentification/EExperimentState.hpp"

namespace DSC
{
    bool HeaterManager::initialize()
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);

        mNewControlModeSetCallback = decltype(mNewControlModeSetCallback)();

        DataManager::updateData(EDataType::HeaterPower, 0.0);
        DataManager::updateData(EDataType::HeaterTemperature, DataManager::UnknownValue);
		DataManager::updateData(EDataType::HeaterTemperatureRtdResistance, DataManager::UnknownValue);
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
		{
			TimerManager::create(1000U, 1000U, []() {
				maxTemperatureController();
			});
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
            //request.power = convertPercentPowerToU16(percent);
			if (0.0F > percent)
			{
				Logger::warning("%s: Requested power: %.2f%% is lower than 0! Adjusting...", getLoggerPrefix().c_str(), percent);
				percent = 0.0F;
			}
			else if (100.0F < percent)
			{
				Logger::warning("%s: Requested power: %.2f%% is greater than 100%%! Adjusting...", getLoggerPrefix().c_str(), percent);
				percent = 100.0F;
			}
            request.power = percent;

            Logger::debug("%s: Setting heater power value to %.2f%%.", getLoggerPrefix().c_str(), request.power);

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
        request.power = convertU16ToPercentPower(power);
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

    bool HeaterManager::setPowerControlProcessPidTunes(double && kp, double && ki, double && kd, double && n)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);

        Logger::debug("%s: Setting power control process PID tunes.", getLoggerPrefix().c_str());

        TSetControllerTunesRequest request;
        request.pid = EPid_ProcessController;
        request.tunes.kp = kp;
        request.tunes.ki = ki;
        request.tunes.kd = kd;
        request.tunes.n = n;

        mSettingPidTunes = std::make_unique<SPidTunes>(request.tunes);

        Nucleo::DeviceCommunicator::send(request, [](TSetControllerTunesResponse && response, bool isNotTimeout){ setControllerTunesResponse(std::move(response)); });

        return true;
    }

	bool HeaterManager::setMaximumAllowedTemperature(float value)
	{
		std::lock_guard<std::mutex> lockGuard(mMtx);

		if (mMaxAllowedTemperature == value)
		{
			Logger::warning("%s: Setting maximum allowed temperature value to %.2f oC skipped. Actual value is the same.", getLoggerPrefix().c_str(), value);
			return true;
		}

		TSetMaximumAllowedTemperatureRequest request;
		request.temperature = value;
		Nucleo::DeviceCommunicator::send(request, [](TSetMaximumAllowedTemperatureResponse && response, bool isNotTimeout) { setMaximumAllowedTemperatureResponse(std::move(response)); });

		return true;
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

    void HeaterManager::startRegisteringTemperatureValue(u16 period)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);

        auto lmpMode = static_cast<ELMP90100Mode>(DataManager::getICMode(EUnitId_LMP90100ControlSystem));
        if (ELMP90100Mode_Off != lmpMode)
        {
            Logger::debug("%s: Starting registering heater temperature value...", getLoggerPrefix().c_str());
            TStartRegisteringDataRequest request;
            request.dataType = ERegisteringDataType_HeaterTemperature;
            request.period = period;
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
		DataManager::updateData(EDataType::HeaterTemperatureRtdResistance, convertTemperatureToRtdResistance(ind.temperature));
    }

    void HeaterManager::controllerDataIndCallback(TControllerDataInd && ind)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);

        switch (ind.type)
        {
            case EControllerDataType_CV:
            {
                DataManager::updateData(EDataType::CVHeaterTemperature, ind.value);
                DataManager::updateData(EDataType::HeaterPower, ind.value);
                break;
            }

            case EControllerDataType_ERR:
            {
                DataManager::updateData(EDataType::ERRHeaterTemperature, ind.value);
                break;
            }

            case EControllerDataType_PV:
            {
                DataManager::updateData(EDataType::PVHeaterTemperature, ind.value);
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

        auto powerPercent = response.power;

        if (response.success)
        {
            Logger::debug("%s: Heater power %.2f%% set.", getLoggerPrefix().c_str(), powerPercent);
            DataManager::updateData(EDataType::HeaterPower, powerPercent);
			mNumberOfFailedRequests = 0U;
        }
        else
        {
            Logger::error("%s: Setting heater power %.2f%% failed!", getLoggerPrefix().c_str(), powerPercent);
			++mNumberOfFailedRequests;
			if (3U == mNumberOfFailedRequests)
			{
				FaultManager::generate(EFaultId_Uart, EUnitId_Nucleo);
			}
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
                    callback(response.power, response.success);
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
            auto controlMode = convertEControlSystemTypeToEControlMode(response.type);
            DataManager::updateControlMode(controlMode);
            DevicePeripherals::UnitsDetector::updateStatus(EUnitId_MCP4716, DevicePeripherals::UnitsDetector::Status::Working);

            switch (controlMode)
            {
                case EControlMode::OpenLoop:
                {
                    DataManager::updateData(EDataType::CVHeaterTemperature, 0.0);
                    DataManager::updateData(EDataType::ERRHeaterTemperature, DataManager::UnknownValue);
                    DataManager::updateData(EDataType::SPHeaterTemperature, DataManager::UnknownValue);
                    break;
                }

                case EControlMode::MFCFeedback :
                case EControlMode::SimpleFeedback:
                {
                    DataManager::updateData(EDataType::SPHeaterTemperature, DataManager::getData(EDataType::HeaterTemperature));
                    break;
                }

                default :
                    break;
            }

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

    void HeaterManager::setControllerTunesResponse(TSetControllerTunesResponse && response)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        
        if (response.success)
        {
            Logger::info("%s: Setting power control process PID tunes.", getLoggerPrefix().c_str());
            Logger::info("%s: Process PID: Kp: %.10f.", getLoggerPrefix().c_str(), mSettingPidTunes->kp);
            Logger::info("%s: Process PID: Ki: %.10f 1/s", getLoggerPrefix().c_str(), mSettingPidTunes->ki);
            Logger::info("%s: Process PID: Kd: %.10f 1/s", getLoggerPrefix().c_str(), mSettingPidTunes->kd);
            Logger::info("%s: Process PID: N: %.10f.", getLoggerPrefix().c_str(), mSettingPidTunes->n);

            DataManager::updateData(EDataType::ProcessPidTuneKp, mSettingPidTunes->kp);
            DataManager::updateData(EDataType::ProcessPidTuneKi, mSettingPidTunes->ki);
            DataManager::updateData(EDataType::ProcessPidTuneKd, mSettingPidTunes->kd);
            DataManager::updateData(EDataType::ProcessPidTuneN, mSettingPidTunes->n);

            mSettingPidTunes.reset();
        }
        else
        {
            Logger::error("%s: Setting power control process PID tunes failed!", getLoggerPrefix().c_str());
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

	void HeaterManager::setMaximumAllowedTemperatureResponse(TSetMaximumAllowedTemperatureResponse && response)
	{
		std::lock_guard<std::mutex> lockGuard(mMtx);

		if (response.success)
		{
			Logger::info("%s: Set maximum allowed temperature to %.2f oC.", getLoggerPrefix().c_str(), response.temperature);
			mMaxAllowedTemperature = response.temperature;
		}
		else
		{
			Logger::error("%s: Setting maximum allowed temperature to %.2f oC failed.", getLoggerPrefix().c_str(), response.temperature);
		}
	}

	void HeaterManager::maxTemperatureController()
	{
		std::lock_guard<std::mutex> lockGuard(mMtx);
		static bool mIsOverheatedReported = false;
		const auto temperature = DataManager::getData(EDataType::HeaterTemperature);
		if (DataManager::UnknownValue == temperature || temperature < mMaxAllowedTemperature)
		{
			if (mIsOverheatedReported)
			{
				Logger::warning("Heater is not overheated! Cancelling fault.", getLoggerPrefix().c_str());
				FaultManager::cancel(EFaultId_TemperatureTooHigh, EUnitId_Heater);
			}
			mIsOverheatedReported = false;
			return;
		}
		if (mIsOverheatedReported)
		{
			return;
		}
		Logger::error("%s: Actual temperature: >= %.2f oC. Maximum allowed temperature: %.2f oC. Overheated!", getLoggerPrefix().c_str(), temperature, mMaxAllowedTemperature);
		FaultManager::generate(EFaultId_TemperatureTooHigh, EUnitId_Heater);
		if (EExperimentState::Running == ModelIdentification::ExperimentManager::getExperimentState())
		{
			ModelIdentification::ExperimentManager::forceStoppingExperiment();
		}
		const auto controlMode = DSC::DataManager::getControlMode();
		if (EControlMode::MFCFeedback == controlMode || EControlMode::SimpleFeedback == controlMode)
		{
			ThreadPool::submit(TaskPriority::High, []() {
				setPowerControlMode(EControlMode::OpenLoop);
			});
		}
		ThreadPool::submit(TaskPriority::High, []() {
			setPower(0);
		});
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

	float HeaterManager::convertTemperatureToRtdResistance(float temperature)
	{
		static const auto z1 = -3.9083E-3F;
		static const auto z2 = 1.758480889E-5F;
		static const auto z3 = -2.310E-9F;
		static const auto z4 = -1.155E-6F;
		return (std::pow(z4 * temperature - z1, 2) - z2) / z3;
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
    std::unique_ptr<SPidTunes> HeaterManager::mSettingPidTunes;
	float HeaterManager::mMaxAllowedTemperature = 380.0F;
	u8 HeaterManager::mNumberOfFailedRequests = 0U;
}