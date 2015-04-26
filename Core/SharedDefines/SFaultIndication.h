#ifndef _S_FAULT_INDICATION_H_

#define _S_FAULT_INDICATION_H_

#ifdef RASPBERRY

#include "EFaultId.h"
#include "EFaultIndicationState.h"
#include "EUnitId.h"

#else

#include "SharedDefines/EFaultId.h"
#include "SharedDefines/EFaultIndicationState.h"
#include "SharedDefines/EUnitId.h"

#endif

typedef struct _SFaultIndication
{
    EFaultId faultId;
    EUnitId faultyUnitId;
    EUnitId faultySubUnitId;
    EFaultIndicationState state;
} SFaultIndication;

#endif
