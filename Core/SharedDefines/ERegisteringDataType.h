#ifndef _E_REGISTERING_DATA_TYPE_H_

#define _E_REGISTERING_DATA_TYPE_H_

#include "../Defines/CommonDefines.hpp"

typedef enum _ERegisteringDataType : u8
{
    ERegisteringDataType_All                                = 0,
    ERegisteringDataType_HeaterTemperature                  = 1,
    ERegisteringDataType_SampleCarrierData                  = 2,
    ERegisteringDataType_ReferenceTemperature               = 3,
    ERegisteringDataType_ControllerData                     = 4
} ERegisteringDataType;

#endif
