#include "DeviceConfigurator.hpp"

#include "ThreadPool.hpp"
#include "TimerManager.hpp"
#include "../FaultManagement/FaultManager.hpp"
#include "../Utilities/Logger.hpp"
#include "../Utilities/Utilities.hpp"
#include "../RaspberryPeripherals/UartManager.hpp"

void DeviceConfigurator::configureEnvironment()
{
    ThreadPool::initialize();
    TimerManager::initialize();
    Logger::initialize();
}

void DeviceConfigurator::configureSystem()
{
    Logger::setLevel(LoggerOutput::GuiTable, LoggerLevel::Debug);
    Logger::info("%s: Started configuring system!", getLoggerPrefix().c_str());

    {
        // LOGGER TEST
        FaultManager::generate(EFaultId_Communication, EUnitId_Nucleo);
        FaultManager::generate(EFaultId_CRCFailure, EUnitId_LMP90100SignalsMeasurement);
        FaultManager::cancel(EFaultId_Communication, EUnitId_Nucleo);
    }

    bool success = true;
    {
        Utilities::conditionalExecutor(success, [](){ return configureRaspberryPeripherals(); });
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
}

void DeviceConfigurator::shutdownSystem()
{

}

bool DeviceConfigurator::configureRaspberryPeripherals()
{
    Logger::debug("%s: Configuring Raspberry peripherals...", getLoggerPrefix().c_str());

    bool success = true;
    {
        Utilities::conditionalExecutor(success, [](){ return UartManager::initialize(); });
    }

    if (success)
    {
        Logger::info("%s: Rasberry peripherals configured!", getLoggerPrefix().c_str());
    }
    else
    {
        Logger::error("%s: Confiduring Raspberry peripherals failed!", getLoggerPrefix().c_str());
    }

    return success;
}

const std::string & DeviceConfigurator::getLoggerPrefix()
{
    static std::string loggerPrefix("DeviceConfigurator");
    return loggerPrefix;
}

