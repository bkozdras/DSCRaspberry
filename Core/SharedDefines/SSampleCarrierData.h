#ifndef _S_SAMPLE_CARRIER_DATA_H_

#define _S_SAMPLE_CARRIER_DATA_H_

#define THERMOCOUPLES_COUNT 5

#include "../SharedDefines/EUnitId.h"

typedef struct _SSampleCarrierData
{
    EUnitId unitId;
    float value;
} SSampleCarrierData;

#endif
