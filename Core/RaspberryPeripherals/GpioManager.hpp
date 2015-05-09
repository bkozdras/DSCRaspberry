#pragma once

#include <boost/noncopyable.hpp>
#include "GPIOClass.hpp"
#include "../HardwareSettings/GpioDefines.hpp"
#include <map>
#include <memory>
#include <mutex>
#include <string>

class GpioManager : public boost::noncopyable
{
    public :

        static bool initialize();
        static bool uninitialize();
        static bool set(RaspberryGpio::Type gpio);
        static bool reset(RaspberryGpio::Type gpio);

    private :

        static std::map<RaspberryGpio::Type, std::unique_ptr<GPIOClass>> mGpioHardwareConnectors;
        static std::mutex mMtx;

        static bool initializeGpios();
        static const std::string & getLoggerPrefix();
};