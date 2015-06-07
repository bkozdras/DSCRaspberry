#pragma once

#include "../Defines/CommonDefines.hpp"

enum class EDataType : u8
{
    HeaterPower,
    HeaterTemperature,
    SPHeaterTemperature,
    PVHeaterTemperature,
    CVHeaterTemperature,
    ERRHeaterTemperature,
    SampleCarrierTemperature,
    ReferenceThermocouple,
    Thermocouple1,
    Thermocouple2,
    Thermocouple3,
    Thermocouple4,
    FilteredThermocouple1,
    FilteredThermocouple2,
    FilteredThermocouple3,
    FilteredThermocouple4,
    SMPCBTemperature,
    FanPower,
    HeaterPowerControlPlotDataSampling,
    HeaterPowerControlFileDataSampling,
    HeaterTemperatureControllingAlgorithmPeriod,
    ActualRealizedSegment,
    IsSegmentsProgramOngoing,
    NumberOfRegisteredSegments,
    ProcessPidTuneKp,
    ProcessPidTuneKi,
    ProcessPidTuneKd,
    ProcessPidTuneN,
    DscDataFileDataSampling,
    FilteringThreshold1,
    FilteringThreshold1Coefficient,
    FilteringThreshold2,
    FilteringThreshold2Coefficient,
    FilteringThreshold3,
    FilteringThreshold3Coefficient,
    FilteringThreshold4,
    FilteringThreshold4Coefficient
};
