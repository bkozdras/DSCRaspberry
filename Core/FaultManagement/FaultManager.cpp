#include "FaultManager.hpp"
#include "../Utilities/Logger.hpp"
#include "../Utilities/ToStringConverter.hpp"
#include "../System/ThreadPool.hpp"
#include <algorithm>

void FaultManager::generate(std::shared_ptr<SFaultIndication> indication)
{
    ThreadPool::submit
    (
        TaskPriority::High,
        [indication]()
        {
            std::lock_guard<std::mutex> lockGuard(mMutex);

            if (EUnitId_Empty == indication->faultySubUnitId)
            {
                Logger::error
                (
                    "%s: Fault %s (%u) reported for unit %s.",
                    getLoggerPrefix().c_str(),
                    ToStringConverter::getFaultId(indication->faultId).c_str(),
                    static_cast<u8>(indication->faultId),
                    ToStringConverter::getUnitId(indication->faultyUnitId).c_str()
                );
            }
            else
            {
                Logger::error
                (
                    "%s: Fault %s (%u) reported for unit %s and subunit %s",
                    getLoggerPrefix().c_str(),
                    ToStringConverter::getFaultId(indication->faultId).c_str(),
                    static_cast<u8>(indication->faultId),
                    ToStringConverter::getUnitId(indication->faultyUnitId).c_str(),
                    ToStringConverter::getUnitId(indication->faultySubUnitId).c_str()
                );
            }

            bool isFaultReported = false;
            std::for_each
            (
                std::begin(mActiveFaults),
                std::end(mActiveFaults),
                [&isFaultReported, &indication](const std::shared_ptr<SFaultIndication> & existedIndication)
                {
                    if (compareFaults(*indication, *existedIndication))
                    {
                        isFaultReported = true;
                    }
                }
            );

            if (!isFaultReported)
            {
                mActiveFaults.push_back(indication);

                for (const auto & callbackTuple : mRegisteredNotificationCallbacks)
                {
                    ThreadPool::submit
                    (
                        TaskPriority::Normal,
                        [callbackTuple, indication]()
                        {
                            if (std::get<1>(callbackTuple))
                            {
                                std::get<1>(callbackTuple)(indication);
                            }
                        }
                    );
                }
            }
        }
    );
}

void FaultManager::generate(SFaultIndication && indication)
{
    ThreadPool::submit
    (
        TaskPriority::High,
        [indication]()
        {
            std::lock_guard<std::mutex> lockGuard(mMutex);

            if (EUnitId_Empty == indication.faultySubUnitId)
            {
                Logger::error
                (
                    "%s: Fault %s (%u) reported for unit %s.",
                    getLoggerPrefix().c_str(),
                    ToStringConverter::getFaultId(indication.faultId).c_str(),
                    static_cast<u8>(indication.faultId),
                    ToStringConverter::getUnitId(indication.faultyUnitId).c_str()
                );
            }
            else
            {
                Logger::error
                (
                    "%s: Fault %s (%u) reported for unit %s and subunit %s",
                    getLoggerPrefix().c_str(),
                    ToStringConverter::getFaultId(indication.faultId).c_str(),
                    static_cast<u8>(indication.faultId),
                    ToStringConverter::getUnitId(indication.faultyUnitId).c_str(),
                    ToStringConverter::getUnitId(indication.faultySubUnitId).c_str()
                );
            }

            auto sharedIndication = std::make_shared<SFaultIndication>(indication);

            bool isFaultReported = false;
            std::for_each
            (
                std::begin(mActiveFaults),
                std::end(mActiveFaults),
                [&isFaultReported, &sharedIndication](const std::shared_ptr<SFaultIndication> & existedIndication)
                {
                    if (compareFaults(*sharedIndication, *existedIndication))
                    {
                        isFaultReported = true;
                    }
                }
            );

            if (!isFaultReported)
            {
                mActiveFaults.push_back(sharedIndication);

                for (const auto & callbackTuple : mRegisteredNotificationCallbacks)
                {
                    ThreadPool::submit
                    (
                        TaskPriority::Normal,
                        [callbackTuple, sharedIndication]()
                        {
                            if (std::get<1>(callbackTuple))
                            {
                                std::get<1>(callbackTuple)(sharedIndication);
                            }
                        }
                    );
                }
            }
        }
    );
}

void FaultManager::generate(EFaultId faultId, EUnitId faultyUnitId, EUnitId faultySubUnitId)
{
    SFaultIndication faultIndication;
    faultIndication.faultId = faultId;
    faultIndication.faultySubUnitId = faultySubUnitId;
    faultIndication.faultyUnitId = faultyUnitId;
    generate(std::move(faultIndication));
}

void FaultManager::cancel(EFaultId faultId, EUnitId faultyUnitId, EUnitId faultySubUnitId)
{
    ThreadPool::submit
    (
        TaskPriority::High,
        [faultId, faultySubUnitId, faultyUnitId]()
        {
            std::lock_guard<std::mutex> lockGuard(mMutex);

            SFaultIndication faultIndication{ faultId, faultyUnitId, faultySubUnitId, EFaultIndicationState_Cancel };

            bool isFaultReported = false;
            auto sharedFaultIndication = std::shared_ptr<SFaultIndication>();

            std::for_each
            (
                std::begin(mActiveFaults),
                std::end(mActiveFaults),
                [&isFaultReported, &faultIndication, &sharedFaultIndication](const std::shared_ptr<SFaultIndication> & existedIndication)
                {
                    if (compareFaults(faultIndication, *existedIndication))
                    {
                        isFaultReported = true;
                        sharedFaultIndication = existedIndication;
                    }
                }
            );

            if (isFaultReported)
            {
                if (EUnitId_Empty == faultIndication.faultySubUnitId)
                {
                    Logger::warning
                    (
                        "%s: Fault %s (%u) cancelled for unit %s.",
                        getLoggerPrefix().c_str(),
                        ToStringConverter::getFaultId(faultIndication.faultId).c_str(),
                        static_cast<u8>(faultIndication.faultId),
                        ToStringConverter::getUnitId(faultIndication.faultyUnitId).c_str()
                    );
                }
                else
                {
                    Logger::warning
                    (
                        "%s: Fault %s (%u) cancelled for unit %s and subunit %s",
                        getLoggerPrefix().c_str(),
                        ToStringConverter::getFaultId(faultIndication.faultId).c_str(),
                        static_cast<u8>(faultIndication.faultId),
                        ToStringConverter::getUnitId(faultIndication.faultyUnitId).c_str(),
                        ToStringConverter::getUnitId(faultIndication.faultySubUnitId).c_str()
                    );
                }

                for (const auto & callbackTuple : mRegisteredNotificationCallbacks)
                {
                    ThreadPool::submit
                    (
                        TaskPriority::Normal,
                        [callbackTuple, sharedFaultIndication]()
                        {
                            if (std::get<2>(callbackTuple))
                            {
                                std::get<2>(callbackTuple)(sharedFaultIndication);
                            }
                        }
                    );
                }

                std::remove_if
                (
                    std::begin(mActiveFaults),
                    std::end(mActiveFaults),
                    [&faultIndication](const std::shared_ptr<SFaultIndication> & indication)
                    {
                        return compareFaults(faultIndication, *indication);
                    }
                );
            }
        }
    );
}

FaultManager::CallbackId FaultManager::registerNotificationCallback
(
    std::function<void(std::shared_ptr<SFaultIndication>)> faultRaisedCallback,
    std::function<void(std::shared_ptr<SFaultIndication>)> faultCancelledCallback
)
{
    std::lock_guard<std::mutex> lockGuard(mMutex);
    auto dynamicId = generateNewId();
    mRegisteredNotificationCallbacks.emplace_back(std::make_tuple(dynamicId, faultRaisedCallback, faultCancelledCallback));
    return dynamicId;
}

void FaultManager::deregisterNotificationCallback(CallbackId id)
{
    std::lock_guard<std::mutex> lockGuard(mMutex);
    std::remove_if
    (
        std::begin(mRegisteredNotificationCallbacks),
        std::end(mRegisteredNotificationCallbacks),
        [id](const std::tuple<FaultManager::CallbackId, Callback, Callback> & callbackTuple)
        {
            return (id == std::get<0>(callbackTuple));
        }
    );
}

bool FaultManager::compareFaults(const SFaultIndication & a, const SFaultIndication & b)
{
    if (!(a.faultId == b.faultId))
    {
        return false;
    }

    if (!(a.faultySubUnitId == b.faultySubUnitId))
    {
        return false;
    }

    if (!(a.faultyUnitId == b.faultyUnitId))
    {
        return false;
    }

    return true;
}

FaultManager::CallbackId FaultManager::generateNewId()
{
    static auto id = 0;
    return ++id;
}

const std::string & FaultManager::getLoggerPrefix()
{
    static std::string loggerPrefix("FaultManager");
    return loggerPrefix;
}

std::mutex FaultManager::mMutex;
std::vector<std::shared_ptr<SFaultIndication>> FaultManager::mActiveFaults;
std::vector<std::tuple<FaultManager::CallbackId, FaultManager::Callback, FaultManager::Callback>> FaultManager::mRegisteredNotificationCallbacks;