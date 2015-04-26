#ifndef _S_SAMPLE_CARRIER_DATA_H_

#define _S_SAMPLE_CARRIER_DATA_H_

#ifdef RASPBERRY

#include "EUnitId.h"

#else

#include "SharedDefines/EUnitId.h"
#include "Defines/CommonDefines.h"

#endif

#define THERMOCOUPLES_COUNT 5

typedef struct _SThermocoupleData
{
    EUnitId thermocouple;
    double milliVoltVoltage;
} SThermocoupleData;

typedef struct _SSampleCarrierData
{
    SThermocoupleData data [THERMOCOUPLES_COUNT];
    double rtdTemperatureValue;
} SSampleCarrierData;

#endif
