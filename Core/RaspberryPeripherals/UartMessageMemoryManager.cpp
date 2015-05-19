#include "UartMessageMemoryManager.hpp"
#include "../SharedDefines/MessagesDefines.h"
#include "../FaultManagement/FaultManager.hpp"
#include "../Utilities/Logger.hpp"

#define ALLOCATE_MACRO(messageName)     case EMessageId_##messageName :             \
                                        {                                           \
                                            return (TByte*) new T##messageName;     \
                                        }

#define DEALLOCATE_MACRO(messageName)   case EMessageId_##messageName :             \
                                        {                                           \
                                            delete (T##messageName *) memoryPtr;    \
                                            return;                                 \
                                        }

#define GET_LENGTH_MACRO(messageName)   case EMessageId_##messageName :             \
                                        {                                           \
                                            return sizeof( T##messageName );        \
                                        }

TByte* UartMessageMemoryManager::allocate(EMessageId messageId)
{
    std::lock_guard<std::mutex> lockGuard(mMtx);

    switch (messageId)
    {
        ALLOCATE_MACRO(CallibreADS1248Request)
        ALLOCATE_MACRO(CallibreADS1248Response)
        ALLOCATE_MACRO(ControllerDataInd)
        ALLOCATE_MACRO(DeregisterSegmentFromProgramRequest)
        ALLOCATE_MACRO(DeregisterSegmentFromProgramResponse)
        ALLOCATE_MACRO(FaultInd)
        ALLOCATE_MACRO(HeaterTemperatureInd)
        ALLOCATE_MACRO(LogInd)
        ALLOCATE_MACRO(PollingRequest)
        ALLOCATE_MACRO(PollingResponse)
        ALLOCATE_MACRO(ReferenceTemperatureInd)
        ALLOCATE_MACRO(RegisterNewSegmentToProgramRequest)
        ALLOCATE_MACRO(RegisterNewSegmentToProgramResponse)
        ALLOCATE_MACRO(ResetUnitRequest)
        ALLOCATE_MACRO(ResetUnitResponse)
        ALLOCATE_MACRO(SampleCarrierDataInd)
        ALLOCATE_MACRO(SegmentsProgramDoneInd)
        ALLOCATE_MACRO(SegmentStartedInd)
        ALLOCATE_MACRO(SetChannelGainADS1248Request)
        ALLOCATE_MACRO(SetChannelGainADS1248Response)
        ALLOCATE_MACRO(SetChannelSamplingSpeedADS1248Request)
        ALLOCATE_MACRO(SetChannelSamplingSpeedADS1248Response)
        ALLOCATE_MACRO(SetControllerTunesRequest)
        ALLOCATE_MACRO(SetControllerTunesResponse)
        ALLOCATE_MACRO(SetControllingAlgorithmExecutionPeriodRequest)
        ALLOCATE_MACRO(SetControllingAlgorithmExecutionPeriodResponse)
        ALLOCATE_MACRO(SetControlSystemTypeRequest)
        ALLOCATE_MACRO(SetControlSystemTypeResponse)
        ALLOCATE_MACRO(SetHeaterPowerRequest)
        ALLOCATE_MACRO(SetHeaterPowerResponse)
        ALLOCATE_MACRO(SetNewDeviceModeADS1248Request)
        ALLOCATE_MACRO(SetNewDeviceModeADS1248Response)
        ALLOCATE_MACRO(SetNewDeviceModeLMP90100ControlSystemRequest)
        ALLOCATE_MACRO(SetNewDeviceModeLMP90100ControlSystemResponse)
        ALLOCATE_MACRO(SetNewDeviceModeLMP90100SignalsMeasurementRequest)
        ALLOCATE_MACRO(SetNewDeviceModeLMP90100SignalsMeasurementResponse)
        ALLOCATE_MACRO(SetProcessModelParametersRequest)
        ALLOCATE_MACRO(SetProcessModelParametersResponse)
        ALLOCATE_MACRO(SetRTDPolynomialCoefficientsRequest)
        ALLOCATE_MACRO(SetRTDPolynomialCoefficientsResponse)
        ALLOCATE_MACRO(StartReferenceTemperatureStabilizationRequest)
        ALLOCATE_MACRO(StartReferenceTemperatureStabilizationResponse)
        ALLOCATE_MACRO(StartRegisteringDataRequest)
        ALLOCATE_MACRO(StartRegisteringDataResponse)
        ALLOCATE_MACRO(StartSegmentProgramRequest)
        ALLOCATE_MACRO(StartSegmentProgramResponse)
        ALLOCATE_MACRO(StopReferenceTemperatureStabilizationRequest)
        ALLOCATE_MACRO(StopReferenceTemperatureStabilizationResponse)
        ALLOCATE_MACRO(StopRegisteringDataRequest)
        ALLOCATE_MACRO(StopRegisteringDataResponse)
        ALLOCATE_MACRO(StopSegmentProgramRequest)
        ALLOCATE_MACRO(StopSegmentProgramResponse)
        ALLOCATE_MACRO(UnitReadyInd)
        ALLOCATE_MACRO(UnexpectedMasterMessageInd)

        default:
        {
            Logger::error("%s: Unknown message memory allocating: ID %u.", getLoggerPrefix().c_str(), messageId);
            FaultManager::generate(EFaultId_System, EUnitId_Raspberry);
            return nullptr;
        }
    }
}

void UartMessageMemoryManager::deallocate(TByte* memoryPtr, EMessageId messageId)
{
    std::lock_guard<std::mutex> lockGuard(mMtx);

    switch (messageId)
    {
        DEALLOCATE_MACRO(CallibreADS1248Request)
        DEALLOCATE_MACRO(CallibreADS1248Response)
        DEALLOCATE_MACRO(ControllerDataInd)
        DEALLOCATE_MACRO(DeregisterSegmentFromProgramRequest)
        DEALLOCATE_MACRO(DeregisterSegmentFromProgramResponse)
        DEALLOCATE_MACRO(FaultInd)
        DEALLOCATE_MACRO(HeaterTemperatureInd)
        DEALLOCATE_MACRO(LogInd)
        DEALLOCATE_MACRO(PollingRequest)
        DEALLOCATE_MACRO(PollingResponse)
        DEALLOCATE_MACRO(ReferenceTemperatureInd)
        DEALLOCATE_MACRO(RegisterNewSegmentToProgramRequest)
        DEALLOCATE_MACRO(RegisterNewSegmentToProgramResponse)
        DEALLOCATE_MACRO(ResetUnitRequest)
        DEALLOCATE_MACRO(ResetUnitResponse)
        DEALLOCATE_MACRO(SampleCarrierDataInd)
        DEALLOCATE_MACRO(SegmentsProgramDoneInd)
        DEALLOCATE_MACRO(SegmentStartedInd)
        DEALLOCATE_MACRO(SetChannelGainADS1248Request)
        DEALLOCATE_MACRO(SetChannelGainADS1248Response)
        DEALLOCATE_MACRO(SetChannelSamplingSpeedADS1248Request)
        DEALLOCATE_MACRO(SetChannelSamplingSpeedADS1248Response)
        DEALLOCATE_MACRO(SetControllerTunesRequest)
        DEALLOCATE_MACRO(SetControllerTunesResponse)
        DEALLOCATE_MACRO(SetControllingAlgorithmExecutionPeriodRequest)
        DEALLOCATE_MACRO(SetControllingAlgorithmExecutionPeriodResponse)
        DEALLOCATE_MACRO(SetControlSystemTypeRequest)
        DEALLOCATE_MACRO(SetControlSystemTypeResponse)
        DEALLOCATE_MACRO(SetHeaterPowerRequest)
        DEALLOCATE_MACRO(SetHeaterPowerResponse)
        DEALLOCATE_MACRO(SetNewDeviceModeADS1248Request)
        DEALLOCATE_MACRO(SetNewDeviceModeADS1248Response)
        DEALLOCATE_MACRO(SetNewDeviceModeLMP90100ControlSystemRequest)
        DEALLOCATE_MACRO(SetNewDeviceModeLMP90100ControlSystemResponse)
        DEALLOCATE_MACRO(SetNewDeviceModeLMP90100SignalsMeasurementRequest)
        DEALLOCATE_MACRO(SetNewDeviceModeLMP90100SignalsMeasurementResponse)
        DEALLOCATE_MACRO(SetProcessModelParametersRequest)
        DEALLOCATE_MACRO(SetProcessModelParametersResponse)
        DEALLOCATE_MACRO(SetRTDPolynomialCoefficientsRequest)
        DEALLOCATE_MACRO(SetRTDPolynomialCoefficientsResponse)
        DEALLOCATE_MACRO(StartReferenceTemperatureStabilizationRequest)
        DEALLOCATE_MACRO(StartReferenceTemperatureStabilizationResponse)
        DEALLOCATE_MACRO(StartRegisteringDataRequest)
        DEALLOCATE_MACRO(StartRegisteringDataResponse)
        DEALLOCATE_MACRO(StartSegmentProgramRequest)
        DEALLOCATE_MACRO(StartSegmentProgramResponse)
        DEALLOCATE_MACRO(StopReferenceTemperatureStabilizationRequest)
        DEALLOCATE_MACRO(StopReferenceTemperatureStabilizationResponse)
        DEALLOCATE_MACRO(StopRegisteringDataRequest)
        DEALLOCATE_MACRO(StopRegisteringDataResponse)
        DEALLOCATE_MACRO(StopSegmentProgramRequest)
        DEALLOCATE_MACRO(StopSegmentProgramResponse)
        DEALLOCATE_MACRO(UnitReadyInd)
        DEALLOCATE_MACRO(UnexpectedMasterMessageInd)

        default:
        {
            Logger::error("%s: Unknown message memory deallocating: ID %u.", getLoggerPrefix().c_str(), messageId);
            FaultManager::generate(EFaultId_System, EUnitId_Raspberry);
        }
    }
}

u16 UartMessageMemoryManager::getLength(EMessageId messageId)
{
    switch (messageId)
    {
        GET_LENGTH_MACRO(CallibreADS1248Request)
        GET_LENGTH_MACRO(CallibreADS1248Response)
        GET_LENGTH_MACRO(ControllerDataInd)
        GET_LENGTH_MACRO(DeregisterSegmentFromProgramRequest)
        GET_LENGTH_MACRO(DeregisterSegmentFromProgramResponse)
        GET_LENGTH_MACRO(FaultInd)
        GET_LENGTH_MACRO(HeaterTemperatureInd)
        GET_LENGTH_MACRO(LogInd)
        GET_LENGTH_MACRO(PollingRequest)
        GET_LENGTH_MACRO(PollingResponse)
        GET_LENGTH_MACRO(ReferenceTemperatureInd)
        GET_LENGTH_MACRO(RegisterNewSegmentToProgramRequest)
        GET_LENGTH_MACRO(RegisterNewSegmentToProgramResponse)
        GET_LENGTH_MACRO(ResetUnitRequest)
        GET_LENGTH_MACRO(ResetUnitResponse)
        GET_LENGTH_MACRO(SampleCarrierDataInd)
        GET_LENGTH_MACRO(SegmentsProgramDoneInd)
        GET_LENGTH_MACRO(SegmentStartedInd)
        GET_LENGTH_MACRO(SetChannelGainADS1248Request)
        GET_LENGTH_MACRO(SetChannelGainADS1248Response)
        GET_LENGTH_MACRO(SetChannelSamplingSpeedADS1248Request)
        GET_LENGTH_MACRO(SetChannelSamplingSpeedADS1248Response)
        GET_LENGTH_MACRO(SetControllerTunesRequest)
        GET_LENGTH_MACRO(SetControllerTunesResponse)
        GET_LENGTH_MACRO(SetControllingAlgorithmExecutionPeriodRequest)
        GET_LENGTH_MACRO(SetControllingAlgorithmExecutionPeriodResponse)
        GET_LENGTH_MACRO(SetControlSystemTypeRequest)
        GET_LENGTH_MACRO(SetControlSystemTypeResponse)
        GET_LENGTH_MACRO(SetHeaterPowerRequest)
        GET_LENGTH_MACRO(SetHeaterPowerResponse)
        GET_LENGTH_MACRO(SetNewDeviceModeADS1248Request)
        GET_LENGTH_MACRO(SetNewDeviceModeADS1248Response)
        GET_LENGTH_MACRO(SetNewDeviceModeLMP90100ControlSystemRequest)
        GET_LENGTH_MACRO(SetNewDeviceModeLMP90100ControlSystemResponse)
        GET_LENGTH_MACRO(SetNewDeviceModeLMP90100SignalsMeasurementRequest)
        GET_LENGTH_MACRO(SetNewDeviceModeLMP90100SignalsMeasurementResponse)
        GET_LENGTH_MACRO(SetProcessModelParametersRequest)
        GET_LENGTH_MACRO(SetProcessModelParametersResponse)
        GET_LENGTH_MACRO(SetRTDPolynomialCoefficientsRequest)
        GET_LENGTH_MACRO(SetRTDPolynomialCoefficientsResponse)
        GET_LENGTH_MACRO(StartReferenceTemperatureStabilizationRequest)
        GET_LENGTH_MACRO(StartReferenceTemperatureStabilizationResponse)
        GET_LENGTH_MACRO(StartRegisteringDataRequest)
        GET_LENGTH_MACRO(StartRegisteringDataResponse)
        GET_LENGTH_MACRO(StartSegmentProgramRequest)
        GET_LENGTH_MACRO(StartSegmentProgramResponse)
        GET_LENGTH_MACRO(StopReferenceTemperatureStabilizationRequest)
        GET_LENGTH_MACRO(StopReferenceTemperatureStabilizationResponse)
        GET_LENGTH_MACRO(StopRegisteringDataRequest)
        GET_LENGTH_MACRO(StopRegisteringDataResponse)
        GET_LENGTH_MACRO(StopSegmentProgramRequest)
        GET_LENGTH_MACRO(StopSegmentProgramResponse)
        GET_LENGTH_MACRO(UnitReadyInd)
        GET_LENGTH_MACRO(UnexpectedMasterMessageInd)

        default:
        {
            Logger::error("%s: Unknown message length: ID %u.", getLoggerPrefix().c_str(), messageId);
            FaultManager::generate(EFaultId_System, EUnitId_Raspberry);
            return 0;
        }
    }
}

const std::string & UartMessageMemoryManager::getLoggerPrefix()
{
    static std::string loggerPrefix = "UartMessageMemoryManager";
    return loggerPrefix;
}

std::mutex UartMessageMemoryManager::mMtx;
