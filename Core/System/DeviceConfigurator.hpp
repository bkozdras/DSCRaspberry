#pragma once

#include <boost/noncopyable.hpp>
#include <string>

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
        static const std::string & getLoggerPrefix();
};