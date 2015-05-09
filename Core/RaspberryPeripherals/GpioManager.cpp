#include "GpioManager.hpp"
#include "../Utilities/Logger.hpp"
#include "../Utilities/Utilities.hpp"

bool GpioManager::initialize()
{
    std::lock_guard<std::mutex> lockGuard(mMtx);
    auto result = initializeGpios();
    if (result)
    {
        Logger::info("%s: Raspberry GPIO Manager initialized!", getLoggerPrefix().c_str());
    }
    else
    {
        Logger::error("%s: Raspberry GPIO Manager not initialized!", getLoggerPrefix().c_str());
    }
    return result;
}

bool GpioManager::uninitialize()
{
    std::lock_guard<std::mutex> lockGuard(mMtx);
    for (auto & gpioHardwareConnector : mGpioHardwareConnectors)
    {
        if (RaspberryGpio::Type::PeripheralsPowerOn == gpioHardwareConnector.first)
        {
            gpioHardwareConnector.second->setval_gpio("0");
        }
        else
        {
            gpioHardwareConnector.second->setval_gpio("1");
        }
    }
    mGpioHardwareConnectors.clear();
    Logger::warning("%s: Raspberry GPIO Manager uninitialized!");

    return true;
}

bool GpioManager::set(RaspberryGpio::Type gpio)
{
    std::lock_guard<std::mutex> lockGuard(mMtx);
    auto result = mGpioHardwareConnectors[gpio]->setval_gpio("1");
    if (result)
    {
        Logger::debug("%s: Gpio %s level set to HIGH.", getLoggerPrefix().c_str(), mGpioHardwareConnectors[gpio]->get_gpionum().c_str());
    }
    else
    {
        Logger::error("%s: Gpio %s level not set to HIGH. Failure!", getLoggerPrefix().c_str(), mGpioHardwareConnectors[gpio]->get_gpionum().c_str());
    }
    return result;
}

bool GpioManager::reset(RaspberryGpio::Type gpio)
{
    std::lock_guard<std::mutex> lockGuard(mMtx);
    auto result = mGpioHardwareConnectors[gpio]->setval_gpio("0");
    if (result)
    {
        Logger::debug("%s: Gpio %s level set to LOW.", getLoggerPrefix().c_str(), mGpioHardwareConnectors[gpio]->get_gpionum().c_str());
    }
    else
    {
        Logger::error("%s: Gpio %s level not set to LOW. Failure!", getLoggerPrefix().c_str(), mGpioHardwareConnectors[gpio]->get_gpionum().c_str());
    }
    return result;
}

bool GpioManager::initializeGpios()
{
    auto initializeHardware =
        [](RaspberryGpio::Type gpioType, std::string defaultLevel)
        {
            mGpioHardwareConnectors.emplace(std::make_pair(gpioType, std::make_unique<GPIOClass>(RaspberryGpio::getNumber(gpioType))));
            if (!mGpioHardwareConnectors[gpioType]->setdir_gpio("out"))
            {
                return false;
            }
            
            return mGpioHardwareConnectors[gpioType]->setval_gpio(defaultLevel);
        };

    bool result = true;

    Utilities::conditionalExecutor(result, [initializeHardware](){ return initializeHardware(RaspberryGpio::Type::PeripheralsPowerOn, "0"); });
    Utilities::conditionalExecutor(result, [initializeHardware](){ return initializeHardware(RaspberryGpio::Type::LedNucleo, "1"); });
    Utilities::conditionalExecutor(result, [initializeHardware](){ return initializeHardware(RaspberryGpio::Type::LedRaspberry, "1"); });
    Utilities::conditionalExecutor(result, [initializeHardware](){ return initializeHardware(RaspberryGpio::Type::LedReferenceTemperatureController, "1"); });
    Utilities::conditionalExecutor(result, [initializeHardware](){ return initializeHardware(RaspberryGpio::Type::LedReserved, "1"); });
    Utilities::conditionalExecutor(result, [initializeHardware](){ return initializeHardware(RaspberryGpio::Type::LedSignalConditionerForControlSystem, "1"); });
    Utilities::conditionalExecutor(result, [initializeHardware](){ return initializeHardware(RaspberryGpio::Type::LedSignalsMeasurementBoard, "1"); });

    if (result)
    {
        Logger::debug("%s: Raspberry GPIOs initialized...", getLoggerPrefix().c_str());
    }
    else
    {
        Logger::error("%s: Raspberry GPIOs not initialized. Failure!", getLoggerPrefix().c_str());
    }

    return result;
}

const std::string & GpioManager::getLoggerPrefix()
{
    static std::string loggerPrefix("GpioManager");
    return loggerPrefix;
}

std::map<RaspberryGpio::Type, std::unique_ptr<GPIOClass>> GpioManager::mGpioHardwareConnectors;
std::mutex GpioManager::mMtx;