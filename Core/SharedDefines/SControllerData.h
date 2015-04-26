#ifndef _S_CONTROLLER_DATA_H_

#define _S_CONTROLLER_DATA_H_

#ifdef RASPBERRY

#include "../Defines/CommonDefines.hpp"

#else

#include "Defines/CommonDefines.h"

#endif

typedef struct _SControllerData
{
    float SP;
    float PV;
    float ERR;
    u16 CV;
} SControllerData;

#endif
