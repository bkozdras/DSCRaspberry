#pragma once

#include <boost/noncopyable.hpp>
#include "../Defines/CommonDefines.hpp"
#include "../SharedDefines/MessagesDefines.h"
#include "EControlMode.hpp"
#include <functional>
#include <memory>
#include <mutex>
#include <string>

namespace DSC
{
    class HeaterManager : public boost::noncopyable
    {
        public :

            static bool initialize();
            static bool setPowerInPercent(float percent, std::function<void(float, bool)> newPowerValueCallback = std::function<void(float, bool)>());
            static bool setPower(u16 power);
            static float getPowerInPercent();
            static u16 getPower();
            static float getTemperature();

            static bool setPowerControlMode(EControlMode mode, std::function<void(EControlMode, bool)> newModeSetCallback = std::function<void(EControlMode, bool)>());
            static EControlMode getPowerControlMode();
            static bool setPowerControlProcessPidTunes(double && kp, double && ki, double && kd, double && n);

            static bool setTemperatureInFeedbackMode(float value);
            static bool setControllingAlgorithmExecutionPeriod(u16 value);

            static void startRegisteringTemperatureValue(u16 period = 1000U);
            static void stopRegisteringTemperatureValue();
            static void startRegisteringControllerValues(u16 period = 1000U);
            static void stopRegisteringControllerValues();

        private :

            static void heaterTemperatureIndCallback(THeaterTemperatureInd && ind);
            static void controllerDataIndCallback(TControllerDataInd && ind);
            static void heaterPowerResponseCallback(TSetHeaterPowerResponse && response);
            static void startRegisteringDataResponseCallback(TStartRegisteringDataResponse && response);
            static void stopRegisteringDataResponseCallback(TStopRegisteringDataResponse && response);
            static void setControlSystemTypeResponse(TSetControlSystemTypeResponse && response);
            static void setControllerTunesResponse(TSetControllerTunesResponse && response);
            static void setHeaterTemperatureInFeedbackModeResponse(TSetHeaterTemperatureInFeedbackModeResponse && response);
            static void setControllingAlgorithmExecutionPeriodResponse(TSetControllingAlgorithmExecutionPeriodResponse && response);

            static const std::string & getLoggerPrefix();

            static u16 convertPercentPowerToU16(float power);
            static float convertU16ToPercentPower(u16 value);
			static float convertTemperatureToRtdResistance(float temperature);
            static EControlMode convertEControlSystemTypeToEControlMode(EControlSystemType type);
            static EControlSystemType convertEControlModeToEControlSystemType(EControlMode mode);

            static std::mutex mMtx;
            static std::function<void(EControlMode, bool)> mNewControlModeSetCallback;
            static std::function<void(float, bool)> mNewPowerValueSetCallback;
            static std::unique_ptr<SPidTunes> mSettingPidTunes;
    };
}