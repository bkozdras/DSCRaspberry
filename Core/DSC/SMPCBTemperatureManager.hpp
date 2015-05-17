#pragma once

#include <boost/noncopyable.hpp>
#include "../Defines/CommonDefines.hpp"
#include "../SharedDefines/MessagesDefines.h"
#include <mutex>
#include <string>

namespace DSC
{
    class SMPCBTemperatureManager : public boost::noncopyable
    {
        public:

            static bool initialize();
            static bool startControlling();
            static bool stopControlling();
            static float getTemperature();

            static void startRegisteringTemperatureValue();
            static void stopRegisteringTemperatureValue();

        private:

            static void referenceTemperatureIndCallback(TReferenceTemperatureInd && ind);
            static void startReferenceTemperatureStabilizationResponseCallback(TStartReferenceTemperatureStabilizationResponse && response);
            static void stopReferenceTemperatureStabilizationResponseCallback(TStopReferenceTemperatureStabilizationResponse && response);
            static void startRegisteringDataResponseCallback(TStartRegisteringDataResponse && response);
            static void stopRegisteringDataResponseCallback(TStopRegisteringDataResponse && response);

            static const std::string & getLoggerPrefix();

            static std::mutex mMtx;
    };
}