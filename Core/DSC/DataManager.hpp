#pragma once

#include <boost/noncopyable.hpp>

#include "../Defines/CommonDefines.hpp"
#include "EControlMode.hpp"
#include "EDataType.hpp"
#include "../SharedDefines/EUnitId.h"
#include <functional>
#include <map>
#include <mutex>
#include <string>

namespace DSC
{
    class DataManager : public boost::noncopyable
    {
        public :

            typedef u16 CallbackId;
            static const double UnknownValue;

            static void updateData(EDataType dataType, double value);
            static double getData(EDataType dataType);
            static void updateControlMode(EControlMode controlMode);
            static EControlMode getControlMode();
            static void updateICMode(EUnitId icId, u8 mode);
            static u8 getICMode(EUnitId icId);
            static void updateUnitAttribute(EUnitId unitId, const std::string & attributeName, const std::string & attribute);
            static bool getUnitAttribute(EUnitId unitId, const std::string & attributeName, std::string & attribute);

            static CallbackId registerNewDataCallback(std::function<void(EDataType, double)> callback);
            static void deregisterNewDataCallback(CallbackId callbackId);
            static CallbackId registerNewControlModeCallback(std::function<void(EControlMode)> callback);
            static void deregisterNewControlModeCallback(CallbackId callbackId);
            static CallbackId registerNewICModeCallback(std::function<void(EUnitId, u8)> callback);
            static void deregisterNewICModeCallback(CallbackId callbackId);
            static CallbackId registerNewUnitAttributeCallback(std::function<void(EUnitId, const std::string &, const std::string &)> callback);
            static void deregisterNewUnitAttributeCallback(CallbackId callbackId);

        private :

            static std::map<EDataType, double> mDataValues;
            static EControlMode mControlMode;
            static std::map<EUnitId, u8> mICModes;
            static std::map<EUnitId, std::map<std::string, std::string>> mUnitAttributes;

            static std::map<CallbackId, std::function<void(EDataType, double)>> mNewDataCallbacks;
            static std::map<CallbackId, std::function<void(EControlMode)>> mNewControlModeCallbacks;
            static std::map<CallbackId, std::function<void(EUnitId, u8)>> mNewICModeCallbacks;
            static std::map<CallbackId, std::function<void(EUnitId, const std::string &, const std::string &)>> mNewUnitAttributeCallbacks;
            static std::mutex mMtx;

            static CallbackId generateUniqueCallbackId();
            static bool isICUnit(EUnitId unitId);

            static const std::string & getLoggerPrefix();
    };
}