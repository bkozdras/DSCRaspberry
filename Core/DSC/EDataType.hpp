#pragma once

#include "../Defines/CommonDefines.hpp"

enum class EDataType : u8
{
    HeaterPower,
    HeaterTemperature,
    SPHeaterTemperature,
    CVHeaterTemperature,
    ERRHeaterTemperature,
    SampleCarrierTemperature,
    ReferenceThermocouple,
    Thermocouple1,
    Thermocouple2,
    Thermocouple3,
    Thermocouple4,
    SMPCBTemperature,
    FanPower,
    HeaterPowerControlPlotDataSampling,
    HeaterPowerControlFileDataSampling,
    HeaterTemperatureControllingAlgorithmPeriod
};
