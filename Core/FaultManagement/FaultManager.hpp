#pragma once

#include <boost/noncopyable.hpp>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <tuple>
#include <vector>

#include "../SharedDefines/SFaultIndication.h"
#include "../SharedDefines/MessagesDefines.h"

class FaultManager : boost::noncopyable
{
    public :

        typedef int CallbackId;

        static bool initialize();

        static void generate(std::shared_ptr<SFaultIndication> indication);
        static void generate(SFaultIndication && indication);
        static void generate(EFaultId faultId, EUnitId faultyUnitId, EUnitId faultySubUnitId = EUnitId_Empty);
        static void cancel(EFaultId faultId, EUnitId faultyUnitId, EUnitId faultySubUnitId = EUnitId_Empty);

        static CallbackId registerNotificationCallback
        (
            std::function<void(std::shared_ptr<SFaultIndication>)> faultRaisedCallback,
            std::function<void(std::shared_ptr<SFaultIndication>)> faultCancelledCallback
        );
        static void deregisterNotificationCallback(CallbackId id);

        static bool compareFaults(const SFaultIndication & a, const SFaultIndication & b);

    private :

        static std::mutex mMutex;
        static std::vector<std::shared_ptr<SFaultIndication>> mActiveFaults;
        typedef std::function<void(std::shared_ptr<SFaultIndication>)> Callback;
        static std::vector<std::tuple<FaultManager::CallbackId, Callback, Callback>> mRegisteredNotificationCallbacks;

        static void newNucleoFaultIndCallback(TFaultInd && faultInd);
        static CallbackId generateNewId();
        static const std::string & getLoggerPrefix();
};