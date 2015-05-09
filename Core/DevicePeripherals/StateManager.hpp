#pragma once

#include <boost/noncopyable.hpp>
#include <mutex>
#include <string>

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

        private :

            static State mState;
            static std::mutex mMtx;

            static const std::string & getLoggerPrefix();
    };
}