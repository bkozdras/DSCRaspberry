#include "DataManager.hpp"
#include "../System/ThreadPool.hpp"
#include "../Utilities/Logger.hpp"
#include "../Utilities/ToStringConverter.hpp"

namespace DSC
{
    void DataManager::updateData(EDataType dataType, double value)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        mDataValues[dataType] = value;
        Logger::debug("%s: Updated Data Type %u with value: %.4f.", getLoggerPrefix().c_str(), static_cast<u8>(dataType), value);

        for (auto & callbackPair : mNewDataCallbacks)
        {
            ThreadPool::submit
            (
                TaskPriority::Normal,
                [callbackPair, dataType, value]()
                {
                    callbackPair.second(dataType, value);
                }
            );
        }
    }

    double DataManager::getData(EDataType dataType)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        auto iter = mDataValues.find(dataType);
        if (std::end(mDataValues) != iter)
        {
            return iter->second;
        }
        else
        {
            return DSC::DataManager::UnknownValue;
        }
    }
    
    void DataManager::updateControlMode(EControlMode controlMode)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        mControlMode = controlMode;

        Logger::debug("%s: Updated Control Mode with value: %u.", getLoggerPrefix().c_str(), static_cast<u8>(controlMode));

        for (auto & callbackPair : mNewControlModeCallbacks)
        {
            ThreadPool::submit
            (
                TaskPriority::Normal,
                [callbackPair, controlMode]()
                {
                    callbackPair.second(controlMode);
                }
            );
        }
    }
    
    EControlMode DataManager::getControlMode()
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        return mControlMode;
    }

    void DataManager::updateICMode(EUnitId icId, u8 mode)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        if (isICUnit(icId))
        {
            mICModes[icId] = mode;
            Logger::debug("%s: Updated IC %s with mode: %u.", getLoggerPrefix().c_str(), ToStringConverter::getUnitId(icId).c_str(), mode);
            for (auto & callbackPair : mNewICModeCallbacks)
            {
                ThreadPool::submit
                (
                    TaskPriority::Normal,
                    [callbackPair, icId, mode]()
                    {
                        callbackPair.second(icId, mode);
                    }
                );
            }
        }
        else
        {
            Logger::warning("%s: Updating IC %s with mode: %u failed. %s is not IC.", getLoggerPrefix().c_str(), ToStringConverter::getUnitId(icId).c_str(), mode, ToStringConverter::getUnitId(icId).c_str());
        }
    }

    u8 DataManager::getICMode(EUnitId icId)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        if (isICUnit(icId))
        {
            return mICModes[icId];
        }

        return 0U;
    }

    void DataManager::updateUnitAttribute(EUnitId unitId, const std::string & attributeName, const std::string & attribute)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        mUnitAttributes[unitId][attributeName] = attribute;
        Logger::debug("%s: Updated Unit %s attribute %s with value: %s.", getLoggerPrefix().c_str(), ToStringConverter::getUnitId(unitId).c_str(), attributeName.c_str(), attribute.c_str());
        for (auto & callbackPair : mNewUnitAttributeCallbacks)
        {
            ThreadPool::submit
            (
                TaskPriority::Normal,
                [callbackPair, unitId, attributeName, attribute]()
                {
                    callbackPair.second(unitId, attributeName, attribute);
                }
            );
        }
    }

    bool DataManager::getUnitAttribute(EUnitId unitId, const std::string & attributeName, std::string & attribute)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        auto unitIt = mUnitAttributes.find(unitId);
        if (std::end(mUnitAttributes) == unitIt)
        {
            Logger::warning("%s: Unit %s attribute %s not found. No attributes stored in %s.", getLoggerPrefix().c_str(), ToStringConverter::getUnitId(unitId).c_str(), attributeName.c_str(), getLoggerPrefix().c_str());
            return false;
        }

        auto attributeIt = unitIt->second.find(attributeName);
        if (std::end(unitIt->second) == attributeIt)
        {
            Logger::warning("%s: Unit %s attribute %s not found. Requested attribute is not stored in %s.", getLoggerPrefix().c_str(), ToStringConverter::getUnitId(unitId).c_str(), attributeName.c_str(), getLoggerPrefix().c_str());
            return false;
        }

        attribute = attributeIt->second;
        return true;
    }

    DataManager::CallbackId DataManager::registerNewDataCallback(std::function<void(EDataType, double)> callback)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        auto callbackId = generateUniqueCallbackId();
        mNewDataCallbacks[callbackId] = callback;
        return callbackId;
    }

    void DataManager::deregisterNewDataCallback(DataManager::CallbackId callbackId)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        auto it = mNewDataCallbacks.find(callbackId);
        if (std::end(mNewDataCallbacks) != it)
        {
            mNewDataCallbacks.erase(it);
        }
    }

    DataManager::CallbackId DataManager::registerNewControlModeCallback(std::function<void(EControlMode)> callback)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        auto callbackId = generateUniqueCallbackId();
        mNewControlModeCallbacks[callbackId] = callback;
        return callbackId;
    }

    void DataManager::deregisterNewControlModeCallback(DataManager::CallbackId callbackId)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        auto it = mNewControlModeCallbacks.find(callbackId);
        if (std::end(mNewControlModeCallbacks) != it)
        {
            mNewControlModeCallbacks.erase(it);
        }
    }

    DataManager::CallbackId DataManager::registerNewICModeCallback(std::function<void(EUnitId, u8)> callback)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        auto callbackId = generateUniqueCallbackId();
        mNewICModeCallbacks[callbackId] = callback;
        return callbackId;
    }

    void DataManager::deregisterNewICModeCallback(CallbackId callbackId)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        auto it = mNewICModeCallbacks.find(callbackId);
        if (std::end(mNewICModeCallbacks) != it)
        {
            mNewICModeCallbacks.erase(it);
        }
    }

    DataManager::CallbackId DataManager::registerNewUnitAttributeCallback(std::function<void(EUnitId, const std::string &, const std::string &)> callback)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        auto callbackId = generateUniqueCallbackId();
        mNewUnitAttributeCallbacks[callbackId] = callback;
        return callbackId;
    }

    void DataManager::deregisterNewUnitAttributeCallback(CallbackId callbackId)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        auto it = mNewUnitAttributeCallbacks.find(callbackId);
        if (std::end(mNewUnitAttributeCallbacks) != it)
        {
            mNewUnitAttributeCallbacks.erase(it);
        }
    }

    DataManager::CallbackId DataManager::generateUniqueCallbackId()
    {
        static u16 uniqueId = 0U;
        return uniqueId++;
    }

    bool DataManager::isICUnit(EUnitId unitId)
    {
        switch (unitId)
        {
            case EUnitId_ADS1248 :
            case EUnitId_LMP90100ControlSystem :
            case EUnitId_LMP90100SignalsMeasurement :
            case EUnitId_MCP4716:
            {
                return true;
            }

            default:
            {
                return false;
            }
        }
    }

    const std::string & DataManager::getLoggerPrefix()
    {
        static std::string loggerPrefix("DataManager");
        return loggerPrefix;
    }

    std::map<EDataType, double> DataManager::mDataValues;
    std::map<EUnitId, u8> DataManager::mICModes;
    std::map<EUnitId, std::map<std::string, std::string>> DataManager::mUnitAttributes;
    std::map<DataManager::CallbackId, std::function<void(EDataType, double)>> DataManager::mNewDataCallbacks;
    std::map<DataManager::CallbackId, std::function<void(EControlMode)>> DataManager::mNewControlModeCallbacks;
    std::map<DataManager::CallbackId, std::function<void(EUnitId, u8)>> DataManager::mNewICModeCallbacks;
    std::map<DataManager::CallbackId, std::function<void(EUnitId, const std::string &, const std::string &)>> DataManager::mNewUnitAttributeCallbacks;
    EControlMode DataManager::mControlMode;
    std::mutex DataManager::mMtx;
    const double DataManager::UnknownValue = 99999999999.0;
}