#pragma once

#include <boost/noncopyable.hpp>
#include "../Defines/CommonDefines.hpp"
#include "../SharedDefines/MessagesDefines.h"
#include <mutex>
#include <string>

namespace DSC
{
    class HeaterManager : public boost::noncopyable
    {
        public :

            static bool initialize();
            static bool setPowerInPercent(float percent);
            static bool setPower(u16 power);
            static float getPowerInPercent();
            static u16 getPower();
            static float getTemperature();

            static void startRegisteringTemperatureValue();
            static void stopRegisteringTemperatureValue();

        private :

            static void heaterTemperatureIndCallback(THeaterTemperatureInd && ind);
            static void heaterPowerResponseCallback(TSetHeaterPowerResponse && response);
            static void startRegisteringDataResponseCallback(TStartRegisteringDataResponse && response);
            static void stopRegisteringDataResponseCallback(TStopRegisteringDataResponse && response);

            static const std::string & getLoggerPrefix();

            static u16 convertPercentPowerToU16(float power);
            static float convertU16ToPercentPower(u16 value);

            static std::mutex mMtx;
    };
}