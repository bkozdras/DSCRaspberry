#pragma once

#include <boost/noncopyable.hpp>
#include <mutex>
#include <string>
#include "../DevicePeripherals/UnitsDetector.hpp"
#include "../DSC/EControlMode.hpp"

class DeviceConfigurator : boost::noncopyable
{
    public :

        static void configureEnvironment();
        static bool configureSystem();
        static void shutdownSystem();

    private :

        static bool configureRaspberryPeripherals();
        static bool configureDevicePeripheralManagers();
        static bool startConfiguringDevicePeripherals();
        static bool configureDSCManagers();
        static const std::string & getLoggerPrefix();

        static void unitReadyIndication(EUnitId unitId, DevicePeripherals::UnitsDetector::Status status);
        static void newIcModeIndication(EUnitId unitId, u8 newMode);
        static void updateUnitAttributeIndication(EUnitId unitId, const std::string & attribute, const std::string & value);
        static void newControlModeSetCallback(EControlMode mode, bool result);

        static std::mutex mMtx;
};