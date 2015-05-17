#pragma once

#include <boost/noncopyable.hpp>
#include "../Defines/CommonDefines.hpp"
#include "../SharedDefines/MessagesDefines.h"
#include "EDataType.hpp"
#include <mutex>
#include <string>

namespace DSC
{
    class SampleCarrierDataManager : public boost::noncopyable
    {
        public:

            static bool initialize();

            static float getTemperature();
            static float getThermocoupleValue(EUnitId thermocouple);

            static void startRegisteringData();
            static void stopRegisteringData();

        private:

            static void sampleCarrierDataIndCallback(TSampleCarrierDataInd && ind);
            static void startRegisteringDataResponseCallback(TStartRegisteringDataResponse && response);
            static void stopRegisteringDataResponseCallback(TStopRegisteringDataResponse && response);

            static const std::string & getLoggerPrefix();
            static bool isUnitThermocouple(EUnitId unitId);
            static EDataType convertThermocoupleUnitIdToDataType(EUnitId unitId);

            static std::mutex mMtx;
    };
}