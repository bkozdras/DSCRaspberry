#include "DeviceConfigurator.hpp"

#include "ThreadPool.hpp"
#include "TimerManager.hpp"
#include "../FaultManagement/FaultManager.hpp"
#include "../Utilities/Logger.hpp"
#include "../Utilities/Utilities.hpp"
#include "../RaspberryPeripherals/UartManager.hpp"
#include "../RaspberryPeripherals/GpioManager.hpp"
#include "../DevicePeripherals/StateManager.hpp"
#include "../DevicePeripherals/UnitsDetector.hpp"
#include "../Nucleo/DeviceCommunicator.hpp"

void DeviceConfigurator::configureEnvironment()
{
    ThreadPool::initialize();
    TimerManager::initialize();
    Logger::initialize();
}

bool DeviceConfigurator::configureSystem()
{
    Logger::setLevel(LoggerOutput::GuiTable, LoggerLevel::Debug);
    Logger::info("%s: Started configuring system!", getLoggerPrefix().c_str());

    bool success = true;
    {
        Utilities::conditionalExecutor(success, [](){ return configureRaspberryPeripherals(); });
        Utilities::conditionalExecutor(success, [](){ return configureDevicePeripheralManagers(); });
        Utilities::conditionalExecutor(success, [](){ return startConfiguringDevicePeripherals(); });
        Utilities::conditionalExecutor(success, [](){ return GpioManager::reset(RaspberryGpio::Type::LedRaspberry); });
    }

    if (success)
    {
        Logger::info("%s: System configured!", getLoggerPrefix().c_str());
        Logger::info("%s: Ready for applying user settings and programs.", getLoggerPrefix().c_str());
    }
    else
    {
        Logger::error("%s: System configuring failure!", getLoggerPrefix().c_str());
        Logger::error("%s: Not ready for applying user settings and programs!", getLoggerPrefix().c_str());
    }

    Logger::setLevel(LoggerOutput::GuiTable, LoggerLevel::Info);

    return success;
}

void DeviceConfigurator::shutdownSystem()
{
    Logger::warning("%s: Shutdowning system...", getLoggerPrefix().c_str());

    GpioManager::uninitialize();

    Logger::warning("%s: System is in shutdown!", getLoggerPrefix().c_str());
}

bool DeviceConfigurator::configureRaspberryPeripherals()
{
    Logger::debug("%s: Configuring Raspberry peripherals...", getLoggerPrefix().c_str());

    bool success = true;
    {
        Utilities::conditionalExecutor(success, [](){ return UartManager::initialize(); });
        Utilities::conditionalExecutor(success, [](){ return GpioManager::initialize(); });
    }

    if (success)
    {
        Logger::info("%s: Raspberry peripherals configured!", getLoggerPrefix().c_str());
    }
    else
    {
        Logger::error("%s: Configuring Raspberry peripherals failed!", getLoggerPrefix().c_str());
    }

    return success;
}

bool DeviceConfigurator::configureDevicePeripheralManagers()
{
    Logger::debug("%s: Configuring device peripheral managers...", getLoggerPrefix().c_str());

    bool success = true;
    {
        Utilities::conditionalExecutor(success, [](){ return Nucleo::DeviceCommunicator::initialize(); });
        Utilities::conditionalExecutor(success, [](){ return DevicePeripherals::UnitsDetector::initialize(); });
        Utilities::conditionalExecutor(success, [](){ return DevicePeripherals::StateManager::initialize(); });
    }

    if (success)
    {
        Logger::info("%s: Device peripheral managers configured!", getLoggerPrefix().c_str());
    }
    else
    {
        Logger::error("%s: Device peripheral managers configuring failed!", getLoggerPrefix().c_str());
    }

    return success;
}

bool DeviceConfigurator::startConfiguringDevicePeripherals()
{
    Logger::debug("%s: Started configuring device peripherals...", getLoggerPrefix().c_str());

    bool success = true;
    {
        Utilities::conditionalExecutor(success, [](){ return DevicePeripherals::StateManager::turnPowerOn(); });
    }

    if (success)
    {
        Logger::info("%s: Starting configuring device peripherals done! Autodetection ongoing...", getLoggerPrefix().c_str());
    }
    else
    {
        Logger::error("%s: Starting configuring device peripherals failed!", getLoggerPrefix().c_str());
    }

    return success;
}

const std::string & DeviceConfigurator::getLoggerPrefix()
{
    static std::string loggerPrefix("DeviceConfigurator");
    return loggerPrefix;
}

