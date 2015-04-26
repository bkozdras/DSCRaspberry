#pragma once

#include <boost/noncopyable.hpp>
#include <string>

class DeviceConfigurator : boost::noncopyable
{
    public :

        static void configureEnvironment();
        static void configureSystem();
        static void shutdownSystem();

    private :

        static bool configureRaspberryPeripherals();
        static const std::string & getLoggerPrefix();
};