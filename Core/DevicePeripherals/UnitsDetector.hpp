#pragma once

#include <boost/noncopyable.hpp>
#include "../SharedDefines/MessagesDefines.h"
#include "../SharedDefines/EUnitId.h"
#include <functional>
#include <map>
#include <mutex>
#include <string>
#include <vector>

namespace DevicePeripherals
{
    class UnitsDetector : boost::noncopyable
    {
        public:

            typedef int CallbackId;

            enum class Status
            {
                Detected,
                Lost,
                Unknown
            };

            static bool initialize();
            static CallbackId registerUnitReadyNotificationCallback(std::function<void(EUnitId, Status)> callback);
            static void deregisterUnitReadyNotificationCallback(CallbackId callbackId);
            Status getStatus(EUnitId unitId);

        private :

            static std::map<EUnitId, Status> mUnitsStatus;
            static std::map<CallbackId, std::function<void(EUnitId, Status)>> mUnitsStatusObservers;
            static std::mutex mMtx;

            static void unitReadyIndCallback(TUnitReadyInd && unitReadyInd);
            static CallbackId generateUniqueCallbackId();
            static const std::string & getLoggerPrefix();
    };
}