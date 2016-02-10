#pragma once

#include <boost/noncopyable.hpp>
#include "../Defines/CommonDefines.hpp"
#include "../SharedDefines/MessagesDefines.h"
#include "EDataType.hpp"
#include <mutex>
#include <string>
#include <vector>
#include <map>

namespace DSC
{
    class SampleCarrierDataManager : public boost::noncopyable
    {
        public:

            enum class FilteringStatus : u8
            {
                Enabled,
                Disabled
            };

            static bool initialize();

            static float getTemperature();
            static float getThermocoupleValue(EUnitId thermocouple);

            static void updateFilterData();
            static void changeThresholdFilteringStatus(FilteringStatus status);
            static void updateAverageFilterData();
            static void changeAverageFilteringStatus(FilteringStatus status);

            static void startRegisteringData();
            static void stopRegisteringData();

        private:

            static void initializeFilterData();
            static void initializeAverageFilterData();
            static void deinitializeFilterData();
            static void deinitializeAverageFilterData();

            static void sampleCarrierDataIndCallback(TSampleCarrierDataInd && ind);
            static void startRegisteringDataResponseCallback(TStartRegisteringDataResponse && response);
            static void stopRegisteringDataResponseCallback(TStopRegisteringDataResponse && response);

            static const std::string & getLoggerPrefix();
            static bool isUnitThermocouple(EUnitId unitId);
            static EDataType convertThermocoupleUnitIdToDataType(EUnitId unitId);
            static EDataType getFilteredDataType(EDataType type);
            static void filterDataAndUpdate(EDataType dataType, const double & newValue);

            static std::mutex mMtx;
            static std::vector<std::pair<double, double>> mFilterData;
            static std::map<EDataType, std::pair<unsigned int, std::vector<double>>> mFilterAverageVector;
            static unsigned int mNAverageCoefficient;
            static unsigned int mXAverageCoefficient;
            static bool mIsThresholdFilteringEnabled;
            static bool mIsAverageFilteringEnabled;
    };
}