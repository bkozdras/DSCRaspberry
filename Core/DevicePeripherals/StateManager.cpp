#include "StateManager.hpp"
#include "../RaspberryPeripherals/GpioManager.hpp"
#include "../Utilities/Logger.hpp"
#include "../Nucleo/DeviceCommunicator.hpp"
#include "../FaultManagement/FaultManager.hpp"
#include "../System/ThreadPool.hpp"

#include <climits>

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

	bool StateManager::startSlavePolling()
	{
		std::lock_guard<std::mutex> lockGuard(mMtx);

		if (mIsPollingOngoing)
		{
			return true;
		}

		Logger::info("%s: Starting slave (Nucleo) polling...", getLoggerPrefix().c_str());

		mPollingTimerId = TimerManager::create(1000U, 10000U, []() {
			TPollingRequest request;
			Nucleo::DeviceCommunicator::send(request, [](TPollingResponse && response, bool isNotTimeout) { pollingResponseCallback(std::move(response)); });
			mPollingFailedTimerId = TimerManager::create(500U, std::numeric_limits<u64>::max(), []() {
				pollingFailedCallback();
			});
			mIsWaitingForPollingResponseOngoing = true;
		});

		mIsPollingOngoing = true;

		return true;
	}

	bool StateManager::stopSlavePolling()
	{
		std::lock_guard<std::mutex> lockGuard(mMtx);
		if (mIsPollingOngoing)
		{
			TimerManager::destroy(mPollingTimerId);
			if (mIsWaitingForPollingResponseOngoing)
			{
				TimerManager::destroy(mPollingFailedTimerId);
				mIsWaitingForPollingResponseOngoing = false;
			}
			mIsPollingOngoing = false;
			Logger::info("%s: Stopping slave (Nucleo) polling...", getLoggerPrefix().c_str());
		}
	}

	void StateManager::pollingFailedCallback()
	{
		std::lock_guard<std::mutex> lockGuard(mMtx);
		++mPollingFailures;
		if (2U <= mPollingFailures)
		{
			Logger::error("%s: Slave (Nucleo) Device: Polling failed!", getLoggerPrefix().c_str());
			FaultManager::generate(EFaultId_Communication, EUnitId_Nucleo);
			mIsWaitingForPollingResponseOngoing = false;
			ThreadPool::submit(TaskPriority::High, []() {
				stopSlavePolling();
			});
		}
		else
		{
			Logger::debug("%s: Single slave (Nucleo )polling failed. Not critical at now.", getLoggerPrefix().c_str());
		}
	}

	void StateManager::pollingResponseCallback(TPollingResponse && response)
	{
		std::lock_guard<std::mutex> lockGuard(mMtx);
		TimerManager::destroy(mPollingFailedTimerId);
		mIsWaitingForPollingResponseOngoing = false;
		mPollingFailures = 0U;
		Logger::debug("%s: Slave (Nucleo) Device: Polling OK.", getLoggerPrefix().c_str());
	}

    const std::string & StateManager::getLoggerPrefix()
    {
        static std::string loggerPrefix("StateManager");
        return loggerPrefix;
    }

    State StateManager::mState;
    std::mutex StateManager::mMtx;
	TimerManager::TimerId StateManager::mPollingTimerId;
	TimerManager::TimerId StateManager::mPollingFailedTimerId;
	bool StateManager::mIsPollingOngoing = false;
	bool StateManager::mIsWaitingForPollingResponseOngoing = false;
	u8 StateManager::mPollingFailures = 0U;
}