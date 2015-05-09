#include "UnitsDetector.hpp"
#include "../Utilities/Logger.hpp"
#include "../Utilities/ToStringConverter.hpp"
#include "../Nucleo/DeviceCommunicator.hpp"
#include "../System/ThreadPool.hpp"
#include "../FaultManagement/FaultManager.hpp"

namespace DevicePeripherals
{
    bool UnitsDetector::initialize()
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);

        mUnitsStatus = decltype(mUnitsStatus)
        {
            std::make_pair(EUnitId_ADS1248, Status::Unknown),
            std::make_pair(EUnitId_DRV595, Status::Unknown),
            std::make_pair(EUnitId_LMP90100ControlSystem, Status::Unknown),
            std::make_pair(EUnitId_LMP90100SignalsMeasurement, Status::Unknown),
            std::make_pair(EUnitId_MCP4716, Status::Unknown),
            std::make_pair(EUnitId_Nucleo, Status::Unknown)
        };

        Nucleo::DeviceCommunicator::registerIndCallback([](TUnitReadyInd && unitReadyInd){ unitReadyIndCallback(std::move(unitReadyInd)); });

        Logger::info("%s: Initialized!", getLoggerPrefix().c_str());
        return true;
    }

    UnitsDetector::CallbackId UnitsDetector::registerUnitReadyNotificationCallback(std::function<void(EUnitId, UnitsDetector::Status)> callback)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        auto callbackId = generateUniqueCallbackId();
        mUnitsStatusObservers[callbackId] = callback;
        return callbackId;
    }

    void UnitsDetector::deregisterUnitReadyNotificationCallback(CallbackId callbackId)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        auto it = mUnitsStatusObservers.find(callbackId);
        if (std::end(mUnitsStatusObservers) != it)
        {
            mUnitsStatusObservers.erase(it);
        }
    }

    UnitsDetector::Status UnitsDetector::getStatus(EUnitId unitId)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        auto it = mUnitsStatus.find(unitId);
        if (std::end(mUnitsStatus) != it)
        {
            return it->second;
        }

        return Status::Unknown;
    }

    void UnitsDetector::unitReadyIndCallback(TUnitReadyInd && unitReadyInd)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);

        auto it = mUnitsStatus.find(unitReadyInd.unitId);

        {
            if (std::end(mUnitsStatus) != it)
            {
                if (unitReadyInd.success)
                {
                    it->second = Status::Detected;
                    Logger::info("%s: Detected new unit %s status: Detected!", getLoggerPrefix().c_str(), ToStringConverter::getUnitId(unitReadyInd.unitId).c_str());
                    FaultManager::cancel(EFaultId_Communication, unitReadyInd.unitId);
                }
                else
                {
                    it->second = Status::Lost;
                    Logger::error("%s: Detected new unit %s status: Lost!", getLoggerPrefix().c_str(), ToStringConverter::getUnitId(unitReadyInd.unitId).c_str());
                    FaultManager::generate(EFaultId_Communication, unitReadyInd.unitId);
                }
            }
        }

        {
            auto unitId = it->first;
            auto status = it->second;
            for (auto & callbackPair : mUnitsStatusObservers)
            {
                ThreadPool::submit
                (
                    TaskPriority::High,
                    [unitId, status, callbackPair]()
                    {
                        callbackPair.second(unitId, status);
                    }
                );
            }
        }
    }

    UnitsDetector::CallbackId UnitsDetector::generateUniqueCallbackId()
    {
        static CallbackId callbackId = 0;
        return callbackId++;
    }

    const std::string & UnitsDetector::getLoggerPrefix()
    {
        static std::string loggerPrefix("UnitsDetector");
        return loggerPrefix;
    }

    std::map<EUnitId, UnitsDetector::Status> UnitsDetector::mUnitsStatus;
    std::map<UnitsDetector::CallbackId, std::function<void(EUnitId, UnitsDetector::Status)>> UnitsDetector::mUnitsStatusObservers;
    std::mutex UnitsDetector::mMtx;
}