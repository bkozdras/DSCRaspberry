#include "StateManager.hpp"
#include "../RaspberryPeripherals/GpioManager.hpp"
#include "../Utilities/Logger.hpp"

namespace DevicePeripherals
{
    bool StateManager::initialize()
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        mState = State::PowerOff;
        auto result = GpioManager::reset(RaspberryGpio::Type::PeripheralsPowerOn);
        if (result)
        {
            Logger::debug("%s: Initialized! Peripherals turned off.", getLoggerPrefix().c_str());
        }
        else
        {
            Logger::error("%s: Initialization failure...");
        }
        return result;
    }

    bool StateManager::turnPowerOn()
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        auto result = GpioManager::set(RaspberryGpio::Type::PeripheralsPowerOn);
        if (result)
        {
            Logger::info("%s: Peripherals turned on!", getLoggerPrefix().c_str());
            mState = State::PowerOn;
        }
        else
        {
            Logger::error("%s: Peripherals turning on failed.", getLoggerPrefix().c_str());
        }

        return result;
    }

    bool StateManager::turnPowerOff()
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        auto result = GpioManager::reset(RaspberryGpio::Type::PeripheralsPowerOn);
        if (result)
        {
            Logger::info("%s: Peripherals turned off!", getLoggerPrefix().c_str());
            mState = State::PowerOff;
        }
        else
        {
            Logger::error("%s: Peripherals turning off failed.", getLoggerPrefix().c_str());
        }

        return result;
    }

    State StateManager::getState()
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        return mState;
    }

    const std::string & StateManager::getLoggerPrefix()
    {
        static std::string loggerPrefix("StateManager");
        return loggerPrefix;
    }

    State StateManager::mState;
    std::mutex StateManager::mMtx;
}