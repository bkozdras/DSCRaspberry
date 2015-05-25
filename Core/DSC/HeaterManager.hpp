#pragma once

#include <boost/noncopyable.hpp>
#include "../Defines/CommonDefines.hpp"
#include "../SharedDefines/MessagesDefines.h"
#include "EControlMode.hpp"
#include <functional>
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

            static void startRegisteringTemperatureValue();
            static void stopRegisteringTemperatureValue();

        private :

            static void heaterTemperatureIndCallback(THeaterTemperatureInd && ind);
            static void heaterPowerResponseCallback(TSetHeaterPowerResponse && response);
            static void startRegisteringDataResponseCallback(TStartRegisteringDataResponse && response);
            static void stopRegisteringDataResponseCallback(TStopRegisteringDataResponse && response);
            static void setControlSystemTypeResponse(TSetControlSystemTypeResponse && response);

            static const std::string & getLoggerPrefix();

            static u16 convertPercentPowerToU16(float power);
            static float convertU16ToPercentPower(u16 value);
            static EControlMode convertEControlSystemTypeToEControlMode(EControlSystemType type);
            static EControlSystemType convertEControlModeToEControlSystemType(EControlMode mode);

            static std::mutex mMtx;
            static std::function<void(EControlMode, bool)> mNewControlModeSetCallback;
            static std::function<void(float, bool)> mNewPowerValueSetCallback;
    };
}