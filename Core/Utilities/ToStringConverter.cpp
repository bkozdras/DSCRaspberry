#include "ToStringConverter.hpp"
#include <map>

namespace ToStringConverter
{
    const std::string & getFaultId(EFaultId faultId)
    {
        static std::map<EFaultId, std::string> faultIdToString = decltype(faultIdToString)
        {
            std::make_pair(EFaultId_Assert, "Assert"),
            std::make_pair(EFaultId_Communication, "Communication"),
            std::make_pair(EFaultId_CRCFailure, "CRCFailure"),
            std::make_pair(EFaultId_I2C, "I2C"),
            std::make_pair(EFaultId_NoMemory, "NoMemory"),
            std::make_pair(EFaultId_OverCurrent, "OverCurrent"),
            std::make_pair(EFaultId_Spi, "Spi"),
            std::make_pair(EFaultId_System, "System"),
            std::make_pair(EFaultId_Uart, "Uart"),
            std::make_pair(EFaultId_WrongData, "WrongData"),
            std::make_pair(EFaultId_Unknown, "Unknown")
        };

        return faultIdToString[faultId];
    }

    const std::string & getUnitId(EUnitId unitId)
    {
        static std::map<EUnitId, std::string> unitIdToString = decltype(unitIdToString)
        {
            std::make_pair(EUnitId_ADS1248, "ADS1248"),
            std::make_pair(EUnitId_DRV595, "DRV595"),
            std::make_pair(EUnitId_Empty, "Empty"),
            std::make_pair(EUnitId_LMP90100ControlSystem, "LMP90100 CS"),
            std::make_pair(EUnitId_LMP90100SignalsMeasurement, "LMP90100 SM"),
            std::make_pair(EUnitId_MCP4716, "MCP4716"),
            std::make_pair(EUnitId_Nucleo, "Nucleo"),
            std::make_pair(EUnitId_Peltier, "Peltier"),
            std::make_pair(EUnitId_Raspberry, "Raspberry"),
            std::make_pair(EUnitId_Rtd1Pt100, "RTD1 Pt100"),
            std::make_pair(EUnitId_Rtd2Pt100, "RTD2 Pt100"),
            std::make_pair(EUnitId_RtdPt1000, "RTD Pt1000"),
            std::make_pair(EUnitId_Thermocouple1, "Thermocouple1"),
            std::make_pair(EUnitId_Thermocouple2, "Thermocouple2"),
            std::make_pair(EUnitId_Thermocouple3, "Thermocouple3"),
            std::make_pair(EUnitId_Thermocouple4, "Thermocouple4"),
            std::make_pair(EUnitId_ThermocoupleReference, "Thermocouple Reference"),
            std::make_pair(EUnitId_Unknown, "Unknown")
        };

        return unitIdToString[unitId];
    }

    const std::string & getMessageId(EMessageId messageId)
    {
        static std::map<EMessageId, std::string> messageIdToString = decltype(messageIdToString)
        {
            std::make_pair(EMessageId_Unknown, "Unknown"),
            std::make_pair(EMessageId_LogInd, "LogInd"),
            std::make_pair(EMessageId_FaultInd, "FaultInd"),
            std::make_pair(EMessageId_PollingRequest, "PollingRequest"),
            std::make_pair(EMessageId_PollingResponse, "PollingResponse"),
            std::make_pair(EMessageId_ResetUnitRequest, "ResetUnitRequest"),
            std::make_pair(EMessageId_ResetUnitResponse, "ResetUnitResponse"),
            std::make_pair(EMessageId_SampleCarrierDataInd, "SampleCarrierDataInd"),
            std::make_pair(EMessageId_HeaterTemperatureInd, "HeaterTemperatureInd"),
            std::make_pair(EMessageId_ReferenceTemperatureInd, "ReferenceTemperatureInd"),
            std::make_pair(EMessageId_ControllerDataInd, "ControllerDataInd"),
            std::make_pair(EMessageId_SetHeaterPowerRequest, "SetHeaterPowerRequest"),
            std::make_pair(EMessageId_SetHeaterPowerResponse, "SetHeaterPowerResponse"),
            std::make_pair(EMessageId_CallibreADS1248Request, "CallibreADS1248Request"),
            std::make_pair(EMessageId_CallibreADS1248Response, "CallibreADS1248Response"),
            std::make_pair(EMessageId_SetChannelGainADS1248Request, "SetChannelGainADS1248Request"),
            std::make_pair(EMessageId_SetChannelGainADS1248Response, "SetChannelGainADS1248Response"),
            std::make_pair(EMessageId_SetChannelSamplingSpeedADS1248Request, "SetChannelSamplingSpeedADS1248Request"),
            std::make_pair(EMessageId_SetChannelSamplingSpeedADS1248Response, "SetChannelSamplingSpeedADS1248Response"),
            std::make_pair(EMessageId_StartRegisteringDataRequest, "StartRegisteringDataRequest"),
            std::make_pair(EMessageId_StartRegisteringDataResponse, "StartRegisteringDataResponse"),
            std::make_pair(EMessageId_StopRegisteringDataRequest, "StopRegisteringDataRequest"),
            std::make_pair(EMessageId_StopRegisteringDataResponse, "StopRegisteringDataResponse"),
            std::make_pair(EMessageId_SetNewDeviceModeLMP90100ControlSystemRequest, "SetNewDeviceModeLMP90100ControlSystemRequest"),
            std::make_pair(EMessageId_SetNewDeviceModeLMP90100ControlSystemResponse, "SetNewDeviceModeLMP90100ControlSystemResponse"),
            std::make_pair(EMessageId_SetNewDeviceModeLMP90100SignalsMeasurementRequest, "SetNewDeviceModeLMP90100SignalsMeasurementRequest"),
            std::make_pair(EMessageId_SetNewDeviceModeLMP90100SignalsMeasurementResponse, "SetNewDeviceModeLMP90100SignalsMeasurementResponse"),
            std::make_pair(EMessageId_SetControlSystemTypeRequest, "SetControlSystemTypeRequest"),
            std::make_pair(EMessageId_SetControlSystemTypeResponse, "SetControlSystemTypeResponse"),
            std::make_pair(EMessageId_SetControllerTunesRequest, "SetControllerTunesRequest"),
            std::make_pair(EMessageId_SetControllerTunesResponse, "SetControllerTunesResponse"),
            std::make_pair(EMessageId_SetProcessModelParametersRequest, "SetProcessModelParametersRequest"),
            std::make_pair(EMessageId_SetProcessModelParametersResponse, "SetProcessModelParametersResponse"),
            std::make_pair(EMessageId_SetControllingAlgorithmExecutionPeriodRequest, "SetControllingAlgorithmExecutionPeriodRequest"),
            std::make_pair(EMessageId_SetControllingAlgorithmExecutionPeriodResponse, "SetControllingAlgorithmExecutionPeriodResponse"),
            std::make_pair(EMessageId_RegisterNewSegmentToProgramRequest, "RegisterNewSegmentToProgramRequest"),
            std::make_pair(EMessageId_RegisterNewSegmentToProgramResponse, "RegisterNewSegmentToProgramResponse"),
            std::make_pair(EMessageId_DeregisterSegmentFromProgramRequest, "DeregisterSegmentFromProgramRequest"),
            std::make_pair(EMessageId_DeregisterSegmentFromProgramResponse, "DeregisterSegmentFromProgramResponse"),
            std::make_pair(EMessageId_StartSegmentProgramRequest, "StartSegmentProgramRequest"),
            std::make_pair(EMessageId_StartSegmentProgramResponse, "StartSegmentProgramResponse"),
            std::make_pair(EMessageId_StopSegmentProgramRequest, "StopSegmentProgramRequest"),
            std::make_pair(EMessageId_StopSegmentProgramResponse, "StopSegmentProgramResponse"),
            std::make_pair(EMessageId_SegmentStartedInd, "SegmentStartedInd"),
            std::make_pair(EMessageId_SegmentsProgramDoneInd, "SegmentsProgramDoneInd"),
            std::make_pair(EMessageId_StartReferenceTemperatureStabilizationRequest, "StartReferenceTemperatureStabilizationRequest"),
            std::make_pair(EMessageId_StartReferenceTemperatureStabilizationResponse, "StartReferenceTemperatureStabilizationResponse"),
            std::make_pair(EMessageId_StopReferenceTemperatureStabilizationResponse, "StopReferenceTemperatureStabilizationResponse"),
            std::make_pair(EMessageId_SetRTDPolynomialCoefficientsRequest, "SetRTDPolynomialCoefficientsRequest"),
            std::make_pair(EMessageId_SetRTDPolynomialCoefficientsResponse, "SetRTDPolynomialCoefficientsResponse"),
            std::make_pair(EMessageId_UnitReadyInd, "UnitReadyInd"),
            std::make_pair(EMessageId_UnexpectedMasterMessageInd, "UnexpectedMasterMessageInd")
        };

        return messageIdToString[messageId];
    }

    const std::string & getADS1248Mode(EADS1248Mode mode)
    {
        static std::map<EADS1248Mode, std::string> modeToString = decltype(modeToString)
        {
            std::make_pair(EADS1248Mode_Off, "Off"),
            std::make_pair(EADS1248Mode_On, "On")
        };

        return modeToString[mode];
    }

    const std::string & getADS1248CallibrationType(EADS1248CallibrationType type)
    {
        static std::map<EADS1248CallibrationType, std::string> typeToString = decltype(typeToString)
        {
            std::make_pair(EADS1248CallibrationType_Idle, "Idle"),
            std::make_pair(EADS1248CallibrationType_OffsetSystem, "Offset System"),
            std::make_pair(EADS1248CallibrationType_SystemGain, "System Gain"),
            std::make_pair(EADS1248CallibrationType_SelfOffset, "Self Offset")
        };

        return typeToString[type];
    }

    const std::string & getADS1248GainValue(EADS1248GainValue value)
    {
        static std::map<EADS1248GainValue, std::string> valueToString = decltype(valueToString)
        {
            std::make_pair(EADS1248GainValue_1, "1"),
            std::make_pair(EADS1248GainValue_2, "2"),
            std::make_pair(EADS1248GainValue_4, "4"),
            std::make_pair(EADS1248GainValue_8, "8"),
            std::make_pair(EADS1248GainValue_16, "16"),
            std::make_pair(EADS1248GainValue_32, "32"),
            std::make_pair(EADS1248GainValue_64, "64"),
            std::make_pair(EADS1248GainValue_128, "128")
        };

        return valueToString[value];
    }

    const std::string & getADS1248SamplingSpeed(EADS1248SamplingSpeed speed)
    {
        static std::map<EADS1248SamplingSpeed, std::string> speedToString = decltype(speedToString)
        {
            std::make_pair(EADS1248SamplingSpeed_5SPS, "5 SPS"),
            std::make_pair(EADS1248SamplingSpeed_10SPS, "10 SPS"),
            std::make_pair(EADS1248SamplingSpeed_20SPS, "20 SPS"),
            std::make_pair(EADS1248SamplingSpeed_40SPS, "40 SPS"),
            std::make_pair(EADS1248SamplingSpeed_80SPS, "80 SPS"),
            std::make_pair(EADS1248SamplingSpeed_160SPS, "160 SPS"),
            std::make_pair(EADS1248SamplingSpeed_320SPS, "320 SPS"),
            std::make_pair(EADS1248SamplingSpeed_640SPS, "640 SPS"),
            std::make_pair(EADS1248SamplingSpeed_1000SPS, "1000 SPS"),
            std::make_pair(EADS1248SamplingSpeed_2000SPS, "2000 SPS")
        };

        return speedToString[speed];
    }

    const std::string & getLMP90100Mode(ELMP90100Mode mode)
    {
        static std::map<ELMP90100Mode, std::string> modeToString = decltype(modeToString)
        {
            std::make_pair(ELMP90100Mode_Off, "Off"),
            std::make_pair(ELMP90100Mode_On_1_6775_SPS, "On 1.6775 SPS"),
            std::make_pair(ELMP90100Mode_On_3_355_SPS, "On 3.355 SPS"),
            std::make_pair(ELMP90100Mode_On_6_71_SPS, "On 6.71 SPS"),
            std::make_pair(ELMP90100Mode_On_13_42_SPS, "On 13.42 SPS"),
            std::make_pair(ELMP90100Mode_On_26_83125_SPS, "On 26.83125 SPS"),
            std::make_pair(ELMP90100Mode_On_53_6625_SPS, "On 53.6625 SPS"),
            std::make_pair(ELMP90100Mode_On_107_325_SPS, "On 107.325 SPS"),
            std::make_pair(ELMP90100Mode_On_214_65_SPS, "On 214.65 SPS")
        };

        return modeToString[mode];
    }
}
