#pragma once

#include <boost/noncopyable.hpp>
#include <mutex>
#include <string>
#include "../System/TimerManager.hpp"
#include "../SharedDefines/MessagesDefines.h"
#include "../Defines/CommonDefines.hpp"

namespace DevicePeripherals
{
    enum class State
    {
        PowerOn,
        PowerOff
    };

    class StateManager : public boost::noncopyable
    {
        public :

            static bool initialize();
            static bool turnPowerOn();
            static bool turnPowerOff();
            static State getState();
			static bool startSlavePolling();
			static bool stopSlavePolling();

        private :

			static void pollingFailedCallback();
			static void pollingResponseCallback(TPollingResponse && response);

            static State mState;
            static std::mutex mMtx;
			static TimerManager::TimerId mPollingTimerId;
			static TimerManager::TimerId mPollingFailedTimerId;
			static bool mIsPollingOngoing;
			static bool mIsWaitingForPollingResponseOngoing;
			static u8 mPollingFailures;

            static const std::string & getLoggerPrefix();
    };
}