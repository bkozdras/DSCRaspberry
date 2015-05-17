#include "IntegratedCircuitsManager.hpp"
#include "../Utilities/Logger.hpp"
#include "../Utilities/ToStringConverter.hpp"
#include "../Nucleo/DeviceCommunicator.hpp"
#include "DataManager.hpp"
#include "../FaultManagement/FaultManager.hpp"
#include "../DevicePeripherals/UnitsDetector.hpp"

namespace DSC
{
    bool IntegratedCircuitsManager::initialize()
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        Logger::info("%s: Initialized!", getLoggerPrefix().c_str());
        return true;
    }

    bool IntegratedCircuitsManager::changeADS1248Mode(EADS1248Mode mode)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        Logger::info("%s: Changing ADS1248 mode to %s.", getLoggerPrefix().c_str(), ToStringConverter::getADS1248Mode(mode).c_str());
        
        TSetNewDeviceModeADS1248Request request;
        request.mode = mode;
        Nucleo::DeviceCommunicator::send(request, [](TSetNewDeviceModeADS1248Response && response, bool isWithTimeout){ setNewDeviceModeADS1248ResponseCallback(std::move(response), isWithTimeout); });
        return true;
    }

    bool IntegratedCircuitsManager::callibreADS1248(EADS1248CallibrationType callibrationType)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        Logger::info("%s: Sending ADS1248 callibration request %s.", getLoggerPrefix().c_str(), ToStringConverter::getADS1248CallibrationType(callibrationType).c_str());

        TCallibreADS1248Request request;
        request.callibrationType = callibrationType;
        Nucleo::DeviceCommunicator::send(request, [](TCallibreADS1248Response && response, bool isWithTimeout){ callibreADS1248ResponseCallback(std::move(response), isWithTimeout); });
        return true;
    }

    bool IntegratedCircuitsManager::setADS1248ChannelGainValue(EADS1248GainValue gainValue)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        Logger::info("%s: Changing ADS1248 channels gain to %s.", getLoggerPrefix().c_str(), ToStringConverter::getADS1248GainValue(gainValue).c_str());

        TSetChannelGainADS1248Request request;
        request.value = gainValue;
        Nucleo::DeviceCommunicator::send(request, [](TSetChannelGainADS1248Response && response, bool isWithTimeout){ setChannelGainADS1248ResponseCallback(std::move(response), isWithTimeout); });
        return true;
    }

    bool IntegratedCircuitsManager::setADS1248ChannelsSamplingSpeed(EADS1248SamplingSpeed samplingSpeed)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        Logger::info("%s: Changing ADS1248 channels multiplexing speed to %s.", getLoggerPrefix().c_str(), ToStringConverter::getADS1248SamplingSpeed(samplingSpeed).c_str());

        TSetChannelSamplingSpeedADS1248Request request;
        request.value = samplingSpeed;
        Nucleo::DeviceCommunicator::send(request, [](TSetChannelSamplingSpeedADS1248Response && response, bool isWithTimeout){ setChannelSamplingSpeedADS1248ResponseCallback(std::move(response), isWithTimeout); });
        return true;
    }

    bool IntegratedCircuitsManager::changeLMP90100Mode(EUnitId lmpUnitId, ELMP90100Mode mode)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        
        if (!isCorrectUnitForLMP90100(lmpUnitId))
        {
            Logger::error("%s: Changing LMP90100 mode: Unit %s is not LMP90100 IC.", getLoggerPrefix().c_str(), ToStringConverter::getUnitId(lmpUnitId).c_str());
            return false;
        }
        
        Logger::info("%s: Changing LMP90100 (%s) mode to %s.", getLoggerPrefix().c_str(), ToStringConverter::getUnitId(lmpUnitId).c_str(), ToStringConverter::getLMP90100Mode(mode).c_str());

        if (EUnitId_LMP90100ControlSystem == lmpUnitId)
        {
            TSetNewDeviceModeLMP90100ControlSystemRequest request;
            request.mode = mode;
            Nucleo::DeviceCommunicator::send(request, [](TSetNewDeviceModeLMP90100ControlSystemResponse && response, bool isWithTimeout){ setNewDeviceModeLMP90100ControlSystemResponseCallback(std::move(response), isWithTimeout); });
        }
        else
        {
            TSetNewDeviceModeLMP90100SignalsMeasurementRequest request;
            request.mode = mode;
            Nucleo::DeviceCommunicator::send(request, [](TSetNewDeviceModeLMP90100SignalsMeasurementResponse && response, bool isWithTimeout){ setNewDeviceModeLMP90100SignalsMeasurementResponseCallback(std::move(response), isWithTimeout); });
        }

        return true;
    }

    void IntegratedCircuitsManager::setNewDeviceModeADS1248ResponseCallback(TSetNewDeviceModeADS1248Response && response, bool isWithoutTimeout)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        if (!isWithoutTimeout)
        {
            Logger::error("%s: No response for SetNewDeviceModeADS1248Request. Timeout!", getLoggerPrefix().c_str());
            FaultManager::generate(EFaultId_Communication, EUnitId_Raspberry, EUnitId_Nucleo);
            return;
        }

        if (response.success)
        {
            Logger::info("%s: Set new ADS1248 mode: %s.", getLoggerPrefix().c_str(), ToStringConverter::getADS1248Mode(response.mode).c_str());
            DataManager::updateICMode(EUnitId_ADS1248, response.mode);
            if (EADS1248Mode_Off != response.mode)
            {
                DevicePeripherals::UnitsDetector::updateStatus(EUnitId_ADS1248, DevicePeripherals::UnitsDetector::Status::Working);
            }
            else
            {
                DevicePeripherals::UnitsDetector::updateStatus(EUnitId_ADS1248, DevicePeripherals::UnitsDetector::Status::Detected);
            }
        }
        else
        {
            Logger::error("%s: Setting new ADS1248 mode: %s failed!", getLoggerPrefix().c_str(), ToStringConverter::getADS1248Mode(response.mode).c_str());
            FaultManager::generate(EFaultId_System, EUnitId_ADS1248);
        }
    }

    void IntegratedCircuitsManager::callibreADS1248ResponseCallback(TCallibreADS1248Response && response, bool isWithoutTimeout)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        if (!isWithoutTimeout)
        {
            Logger::error("%s: No response for CallibreADS1248Request. Timeout!", getLoggerPrefix().c_str());
            FaultManager::generate(EFaultId_Communication, EUnitId_Raspberry, EUnitId_Nucleo);
            return;
        }

        if (response.success)
        {
            Logger::info("%s: ADS1248 channels callibration: %s done.", getLoggerPrefix().c_str(), ToStringConverter::getADS1248CallibrationType(response.callibrationType).c_str());
            DataManager::updateUnitAttribute(EUnitId_ADS1248, "CallibrationDone", ToStringConverter::getADS1248CallibrationType(response.callibrationType));
        }
        else
        {
            Logger::error("%s: ADS1248 channels callibration: %s failed!", getLoggerPrefix().c_str(), ToStringConverter::getADS1248CallibrationType(response.callibrationType).c_str());
            FaultManager::generate(EFaultId_System, EUnitId_ADS1248);
        }
    }

    void IntegratedCircuitsManager::setChannelGainADS1248ResponseCallback(TSetChannelGainADS1248Response && response, bool isWithoutTimeout)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        if (!isWithoutTimeout)
        {
            Logger::error("%s: No response for SetChannelGainADS1248Request. Timeout!", getLoggerPrefix().c_str());
            FaultManager::generate(EFaultId_Communication, EUnitId_Raspberry, EUnitId_Nucleo);
            return;
        }

        if (response.success)
        {
            Logger::info("%s: Set new ADS1248 channels gain: %s.", getLoggerPrefix().c_str(), ToStringConverter::getADS1248GainValue(response.value).c_str());
            DataManager::updateUnitAttribute(EUnitId_ADS1248, "ChannelsGain", ToStringConverter::getADS1248GainValue(response.value));
        }
        else
        {
            Logger::error("%s: Setting new ADS1248 channels gain: %s failed!", getLoggerPrefix().c_str(), ToStringConverter::getADS1248GainValue(response.value).c_str());
            FaultManager::generate(EFaultId_System, EUnitId_ADS1248);
        }
    }

    void IntegratedCircuitsManager::setChannelSamplingSpeedADS1248ResponseCallback(TSetChannelSamplingSpeedADS1248Response && response, bool isWithoutTimeout)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        if (!isWithoutTimeout)
        {
            Logger::error("%s: No response for SetChannelSamplingSpeedADS1248Request. Timeout!", getLoggerPrefix().c_str());
            FaultManager::generate(EFaultId_Communication, EUnitId_Raspberry, EUnitId_Nucleo);
            return;
        }

        if (response.success)
        {
            Logger::info("%s: Set new ADS1248 channels multiplexing sampling speed: %s.", getLoggerPrefix().c_str(), ToStringConverter::getADS1248SamplingSpeed(response.value).c_str());
            DataManager::updateUnitAttribute(EUnitId_ADS1248, "ChannelsSamplingSpeed", ToStringConverter::getADS1248SamplingSpeed(response.value));
        }
        else
        {
            Logger::error("%s: Setting new ADS1248 channels multiplexing sampling speed: %s failed!", getLoggerPrefix().c_str(), ToStringConverter::getADS1248SamplingSpeed(response.value).c_str());
            FaultManager::generate(EFaultId_System, EUnitId_ADS1248);
        }
    }

    void IntegratedCircuitsManager::setNewDeviceModeLMP90100ControlSystemResponseCallback(TSetNewDeviceModeLMP90100ControlSystemResponse && response, bool isWithoutTimeout)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        if (!isWithoutTimeout)
        {
            Logger::error("%s: No response for SetNewDeviceModeLMP90100ControlSystemRequest. Timeout!", getLoggerPrefix().c_str());
            FaultManager::generate(EFaultId_Communication, EUnitId_Raspberry, EUnitId_Nucleo);
            return;
        }

        if (response.success)
        {
            Logger::info("%s: Set new LMP90100 CS mode: %s.", getLoggerPrefix().c_str(), ToStringConverter::getLMP90100Mode(response.mode).c_str());
            DataManager::updateICMode(EUnitId_LMP90100ControlSystem, response.mode);
            DataManager::updateICMode(EUnitId_RtdPt1000, response.mode);
            if (ELMP90100Mode_Off != response.mode)
            {
                DevicePeripherals::UnitsDetector::updateStatus(EUnitId_LMP90100ControlSystem, DevicePeripherals::UnitsDetector::Status::Working);
            }
            else
            {
                DevicePeripherals::UnitsDetector::updateStatus(EUnitId_LMP90100ControlSystem, DevicePeripherals::UnitsDetector::Status::Detected);
            }
        }
        else
        {
            Logger::error("%s: Setting new LMP90100 CS mode: %s failed!", getLoggerPrefix().c_str(), ToStringConverter::getLMP90100Mode(response.mode).c_str());
            FaultManager::generate(EFaultId_System, EUnitId_LMP90100ControlSystem);
        }
    }

    void IntegratedCircuitsManager::setNewDeviceModeLMP90100SignalsMeasurementResponseCallback(TSetNewDeviceModeLMP90100SignalsMeasurementResponse && response, bool isWithoutTimeout)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        if (!isWithoutTimeout)
        {
            Logger::error("%s: No response for SetNewDeviceModeLMP90100SignalsMeasurementRequest. Timeout!", getLoggerPrefix().c_str());
            FaultManager::generate(EFaultId_Communication, EUnitId_Raspberry, EUnitId_Nucleo);
            return;
        }

        if (response.success)
        {
            Logger::info("%s: Set new LMP90100 SM mode: %s.", getLoggerPrefix().c_str(), ToStringConverter::getLMP90100Mode(response.mode).c_str());
            DataManager::updateICMode(EUnitId_LMP90100SignalsMeasurement, response.mode);
            if (ELMP90100Mode_Off != response.mode)
            {
                DevicePeripherals::UnitsDetector::updateStatus(EUnitId_LMP90100SignalsMeasurement, DevicePeripherals::UnitsDetector::Status::Working);
            }
            else
            {
                DevicePeripherals::UnitsDetector::updateStatus(EUnitId_LMP90100SignalsMeasurement, DevicePeripherals::UnitsDetector::Status::Detected);
            }
        }
        else
        {
            Logger::error("%s: Setting new LMP90100 CS mode: %s failed!", getLoggerPrefix().c_str(), ToStringConverter::getLMP90100Mode(response.mode).c_str());
            FaultManager::generate(EFaultId_System, EUnitId_LMP90100SignalsMeasurement);
        }
    }

    bool IntegratedCircuitsManager::isCorrectUnitForLMP90100(EUnitId unitId)
    {
        switch (unitId)
        {
            case EUnitId_LMP90100ControlSystem :
            case EUnitId_LMP90100SignalsMeasurement :
                return true;

            default :
                return false;
        }
    }

    const std::string & IntegratedCircuitsManager::getLoggerPrefix()
    {
        static std::string loggerPrefix("IntegratedCircuitsManager");
        return loggerPrefix;
    }

    std::mutex IntegratedCircuitsManager::mMtx;
}