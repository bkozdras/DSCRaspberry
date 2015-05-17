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
                Working,
                Unknown
            };

            static bool initialize();
            static CallbackId registerUnitReadyNotificationCallback(std::function<void(EUnitId, Status)> callback);
            static void deregisterUnitReadyNotificationCallback(CallbackId callbackId);
            static Status getStatus(EUnitId unitId);
            static void updateStatus(EUnitId unitId, Status status);
            static bool areAllUnitsNotLost();

        private :

            static std::map<EUnitId, Status> mUnitsStatus;
            static std::map<CallbackId, std::function<void(EUnitId, Status)>> mUnitsStatusObservers;
            static std::mutex mMtx;

            static void unitReadyIndCallback(TUnitReadyInd && unitReadyInd);
            static CallbackId generateUniqueCallbackId();
            static const std::string & getStatusString(Status status);
            static const std::string & getLoggerPrefix();
    };
}