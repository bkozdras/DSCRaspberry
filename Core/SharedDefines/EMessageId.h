#ifndef _E_MESSAGE_ID_H_

#define _E_MESSAGE_ID_H_

#include "../Defines/CommonDefines.hpp"

typedef enum _EMessageId : u8
{
    EMessageId_Unknown                                                      = 0,
    EMessageId_LogInd                                                       = 1,
    EMessageId_FaultInd                                                     = 2,
    EMessageId_PollingRequest                                               = 3,
    EMessageId_PollingResponse                                              = 4,
    EMessageId_ResetUnitRequest                                             = 5,
    EMessageId_ResetUnitResponse                                            = 6,
    EMessageId_SampleCarrierDataInd                                         = 7,
    EMessageId_HeaterTemperatureInd                                         = 8,
    EMessageId_ReferenceTemperatureInd                                      = 9,
    EMessageId_ControllerDataInd                                            = 10,
    EMessageId_SetHeaterPowerRequest                                        = 11,
    EMessageId_SetHeaterPowerResponse                                       = 12,
    EMessageId_CallibreADS1248Request                                       = 13,
    EMessageId_CallibreADS1248Response                                      = 14,
    EMessageId_SetChannelGainADS1248Request                                 = 15,
    EMessageId_SetChannelGainADS1248Response                                = 16,
    EMessageId_SetChannelSamplingSpeedADS1248Request                        = 17,
    EMessageId_SetChannelSamplingSpeedADS1248Response                       = 18,
    EMessageId_StartRegisteringDataRequest                                  = 19,
    EMessageId_StartRegisteringDataResponse                                 = 20,
    EMessageId_StopRegisteringDataRequest                                   = 21,
    EMessageId_StopRegisteringDataResponse                                  = 22,
    EMessageId_SetNewDeviceModeLMP90100ControlSystemRequest                 = 23,
    EMessageId_SetNewDeviceModeLMP90100ControlSystemResponse                = 24,
    EMessageId_SetNewDeviceModeLMP90100SignalsMeasurementRequest            = 25,
    EMessageId_SetNewDeviceModeLMP90100SignalsMeasurementResponse           = 26,
    EMessageId_SetControlSystemTypeRequest                                  = 27,
    EMessageId_SetControlSystemTypeResponse                                 = 28,
    EMessageId_SetControllerTunesRequest                                    = 29,
    EMessageId_SetControllerTunesResponse                                   = 30,
    EMessageId_SetProcessModelParametersRequest                             = 31,
    EMessageId_SetProcessModelParametersResponse                            = 32,
    EMessageId_SetControllingAlgorithmExecutionPeriodRequest                = 33,
    EMessageId_SetControllingAlgorithmExecutionPeriodResponse               = 34,
    EMessageId_RegisterNewSegmentToProgramRequest                           = 35,
    EMessageId_RegisterNewSegmentToProgramResponse                          = 36,
    EMessageId_DeregisterSegmentFromProgramRequest                          = 37,
    EMessageId_DeregisterSegmentFromProgramResponse                         = 38,
    EMessageId_StartSegmentProgramRequest                                   = 39,
    EMessageId_StartSegmentProgramResponse                                  = 40,
    EMessageId_StopSegmentProgramRequest                                    = 41,
    EMessageId_StopSegmentProgramResponse                                   = 42,
    EMessageId_SegmentStartedInd                                            = 43,
    EMessageId_SegmentsProgramDoneInd                                       = 44,
    EMessageId_StartReferenceTemperatureStabilizationRequest                = 45,
    EMessageId_StartReferenceTemperatureStabilizationResponse               = 46,
    EMessageId_StopReferenceTemperatureStabilizationRequest                 = 47,
    EMessageId_StopReferenceTemperatureStabilizationResponse                = 48,
    EMessageId_SetRTDPolynomialCoefficientsRequest                          = 49,
    EMessageId_SetRTDPolynomialCoefficientsResponse                         = 50,
    EMessageId_UnitReadyInd                                                 = 51,
    EMessageId_UnexpectedMasterMessageInd                                   = 99
} EMessageId;

#endif
