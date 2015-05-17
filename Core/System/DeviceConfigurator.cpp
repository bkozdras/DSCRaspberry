#include "DeviceConfigurator.hpp"

#include "ThreadPool.hpp"
#include "TimerManager.hpp"
#include "../FaultManagement/FaultManager.hpp"
#include "../Utilities/Logger.hpp"
#include "../Utilities/Utilities.hpp"
#include "../Utilities/ToStringConverter.hpp"
#include "../RaspberryPeripherals/UartManager.hpp"
#include "../RaspberryPeripherals/GpioManager.hpp"
#include "../DevicePeripherals/StateManager.hpp"
#include "../DevicePeripherals/UnitsDetector.hpp"
#include "../Nucleo/DeviceCommunicator.hpp"
#include "../DSC/DataManager.hpp"
#include "../DSC/HeaterManager.hpp"
#include "../DSC/IntegratedCircuitsManager.hpp"
#include "../DSC/SampleCarrierManager.hpp"
#include "../DSC/SegmentsManager.hpp"
#include "../DSC/SMPCBTemperatureManager.hpp"
#include "../System/SystemErrorsManager.hpp"

void DeviceConfigurator::configureEnvironment()
{
    std::lock_guard<std::mutex> lockGuard(mMtx);

    ThreadPool::initialize();
    TimerManager::initialize();
    Logger::initialize();
    SystemErrorsManager::initialize();
}

bool DeviceConfigurator::configureSystem()
{
    std::lock_guard<std::mutex> lockGuard(mMtx);

    Logger::setLevel(LoggerOutput::GuiTable, LoggerLevel::Debug);
    Logger::info("%s: Started configuring system!", getLoggerPrefix().c_str());

    bool success = true;
    {
        Utilities::conditionalExecutor(success, [](){ return configureRaspberryPeripherals(); });
        Utilities::conditionalExecutor(success, [](){ return configureDevicePeripheralManagers(); });
        Utilities::conditionalExecutor(success, [](){ return configureDSCManagers(); });
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

    //Logger::setLevel(LoggerOutput::GuiTable, LoggerLevel::Info);

    return success;
}

void DeviceConfigurator::shutdownSystem()
{
    std::lock_guard<std::mutex> lockGuard(mMtx);

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

bool DeviceConfigurator::configureDSCManagers()
{
    Logger::debug("%s: Configuring DSC managers...", getLoggerPrefix().c_str());

    bool success = true;
    {
        Utilities::conditionalExecutor(success, [](){ return DSC::HeaterManager::initialize(); });
        Utilities::conditionalExecutor(success, [](){ return DSC::IntegratedCircuitsManager::initialize(); });
        Utilities::conditionalExecutor(success, [](){ return DSC::SampleCarrierDataManager::initialize(); });
        Utilities::conditionalExecutor(success, [](){ return DSC::SMPCBTemperatureManager::initialize(); });

        if (success)
        {
            DevicePeripherals::UnitsDetector::registerUnitReadyNotificationCallback
            (
                [](EUnitId unitId, DevicePeripherals::UnitsDetector::Status status)
                {
                    unitReadyIndication(unitId, status);
                }
            );

            DSC::DataManager::registerNewICModeCallback
            (
                [](EUnitId unitId, u8 newMode)
                {
                    newIcModeIndication(unitId, newMode);
                }
            );
        }
    }

    if (success)
    {
        Logger::info("%s: DSC managers configuring done!", getLoggerPrefix().c_str());
    }
    else
    {
        Logger::error("%s: Configuring DSC managers failed!", getLoggerPrefix().c_str());
    }

    return success;
}

const std::string & DeviceConfigurator::getLoggerPrefix()
{
    static std::string loggerPrefix("DeviceConfigurator");
    return loggerPrefix;
}

void DeviceConfigurator::unitReadyIndication(EUnitId unitId, DevicePeripherals::UnitsDetector::Status status)
{
    std::lock_guard<std::mutex> lockGuard(mMtx);

    static auto areConfigured = false;

    if (!areConfigured && DevicePeripherals::UnitsDetector::areAllUnitsNotLost())
    {
        Logger::info("%s: All units are properly detected!", getLoggerPrefix().c_str());
        Logger::info("%s: Starting collecting DSC / Device data...", getLoggerPrefix().c_str());
        DSC::IntegratedCircuitsManager::changeLMP90100Mode(EUnitId_LMP90100ControlSystem, ELMP90100Mode_On_1_6775_SPS);
        areConfigured = true;
    }
}

void DeviceConfigurator::newIcModeIndication(EUnitId unitId, u8 newMode)
{
    std::lock_guard<std::mutex> lockGuard(mMtx);

    switch (unitId)
    {
        case EUnitId_LMP90100ControlSystem:
        {
            auto lmpMode = static_cast<ELMP90100Mode>(newMode);
            Logger::debug("%s: New %s mode %s set!", getLoggerPrefix().c_str(), ToStringConverter::getUnitId(unitId).c_str(), ToStringConverter::getLMP90100Mode(lmpMode).c_str());

            {
                DSC::HeaterManager::startRegisteringTemperatureValue();
            }
            {
                DSC::IntegratedCircuitsManager::changeLMP90100Mode(EUnitId_LMP90100SignalsMeasurement, ELMP90100Mode_On_1_6775_SPS);
            }

            break;
        }

        case EUnitId_LMP90100SignalsMeasurement :
        {
            auto lmpMode = static_cast<ELMP90100Mode>(newMode);
            Logger::debug("%s: New %s mode %s set!", getLoggerPrefix().c_str(), ToStringConverter::getUnitId(unitId).c_str(), ToStringConverter::getLMP90100Mode(lmpMode).c_str());

            {
                DSC::SMPCBTemperatureManager::startRegisteringTemperatureValue();
            }
            {
                DSC::IntegratedCircuitsManager::changeADS1248Mode(EADS1248Mode_On);
            }

            break;
        }

        case EUnitId_ADS1248:
        {
            auto ads1248Mode = static_cast<EADS1248Mode>(newMode);
            Logger::debug("%s: New %s mode %s set!", getLoggerPrefix().c_str(), ToStringConverter::getUnitId(unitId).c_str(), ToStringConverter::getADS1248Mode(ads1248Mode).c_str());
            
            {
                DSC::IntegratedCircuitsManager::setADS1248ChannelGainValue(EADS1248GainValue_128);
            }
        }

        default :
            break;
    }
}

void DeviceConfigurator::updateUnitAttributeIndication(EUnitId unitId, const std::string & attribute, const std::string & value)
{
    std::lock_guard<std::mutex> lockGuard(mMtx);

    switch (unitId)
    {
        case EUnitId_ADS1248:
        {
            if ("CallibrationDone" == attribute)
            {
                if ("System Gain" == value)
                {
                    Logger::debug("%s: %s unit callibration type: %s done!", getLoggerPrefix().c_str(), ToStringConverter::getUnitId(unitId).c_str(), value.c_str());

                    {
                        DSC::IntegratedCircuitsManager::callibreADS1248(EADS1248CallibrationType_SelfOffset);
                    }
                }
                else if ("Self Offset" == value)
                {
                    Logger::debug("%s: %s unit callibration type: %s done!", getLoggerPrefix().c_str(), ToStringConverter::getUnitId(unitId).c_str(), value.c_str());

                    {
                        DSC::DataManager::updateUnitAttribute(unitId, "CallibrationDone", "All");
                    }
                    {
                        DSC::SampleCarrierDataManager::startRegisteringData();
                    }
                }
            }
            else if ("ChannelsGain" == attribute)
            {
                Logger::debug("%s: %s unit channels gain set to %s.", getLoggerPrefix().c_str(), ToStringConverter::getUnitId(unitId).c_str(), value.c_str());

                {
                    DSC::IntegratedCircuitsManager::setADS1248ChannelsSamplingSpeed(EADS1248SamplingSpeed_5SPS);
                }
            }
            else if ("ChannelsSamplingSpeed" == attribute)
            {
                Logger::debug("%s: %s unit channels sampling speed set to %s.", getLoggerPrefix().c_str(), ToStringConverter::getUnitId(unitId).c_str(), value.c_str());

                {
                    DSC::IntegratedCircuitsManager::callibreADS1248(EADS1248CallibrationType_SystemGain);
                }
            }

            break;
        }

        default :
            break;
    }

}

std::mutex DeviceConfigurator::mMtx;

