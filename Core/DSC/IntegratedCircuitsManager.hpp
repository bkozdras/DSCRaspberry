#pragma once

#include <boost/noncopyable.hpp>
#include "../SharedDefines/ADS1248Types.h"
#include "../SharedDefines/LMP90100Types.h"
#include "../SharedDefines/EUnitId.h"
#include "../SharedDefines/MessagesDefines.h"

#include <mutex>
#include <string>

namespace DSC
{
    class IntegratedCircuitsManager : public boost::noncopyable
    {
        public :

            static bool initialize();

            static bool changeADS1248Mode(EADS1248Mode mode);
            static bool callibreADS1248(EADS1248CallibrationType callibrationType);
            static bool setADS1248ChannelGainValue(EADS1248GainValue gainValue);
            static bool setADS1248ChannelsSamplingSpeed(EADS1248SamplingSpeed samplingSpeed);

            static bool changeLMP90100Mode(EUnitId lmpUnitId, ELMP90100Mode mode);

        private :

            static void setNewDeviceModeADS1248ResponseCallback(TSetNewDeviceModeADS1248Response && response, bool isWithoutTimeout);
            static void callibreADS1248ResponseCallback(TCallibreADS1248Response && response, bool isWithoutTimeout);
            static void setChannelGainADS1248ResponseCallback(TSetChannelGainADS1248Response && response, bool isWithoutTimeout);
            static void setChannelSamplingSpeedADS1248ResponseCallback(TSetChannelSamplingSpeedADS1248Response && response, bool isWithoutTimeout);
            static void setNewDeviceModeLMP90100ControlSystemResponseCallback(TSetNewDeviceModeLMP90100ControlSystemResponse && response, bool isWithoutTimeout);
            static void setNewDeviceModeLMP90100SignalsMeasurementResponseCallback(TSetNewDeviceModeLMP90100SignalsMeasurementResponse && response, bool isWithoutTimeout);

            static bool isCorrectUnitForLMP90100(EUnitId unitId);

            static const std::string & getLoggerPrefix();

            static std::mutex mMtx;
    };
}