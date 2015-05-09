#ifndef _E_FAULT_INDICATION_STATE_H_

#define _E_FAULT_INDICATION_STATE_H_

#include "../Defines/CommonDefines.hpp"

typedef enum _EFaultIndicationState : u8
{
    EFaultIndicationState_Start     = 0,
    EFaultIndicationState_Cancel    = 1
} EFaultIndicationState;

#endif
