#ifndef _E_LOG_SEVERITY_H_

#define _E_LOG_SEVERITY_H_

#include "../Defines/CommonDefines.hpp"

typedef enum _ELogSeverity : u8
{
    ELogSeverity_DebugSystem                    = 0,
    ELogSeverity_DebugSystemMasterDataExtended  = 1,
    ELogSeverity_Debug                          = 2,
    ELogSeverity_Info                           = 3,
    ELogSeverity_Warning                        = 4,
    ELogSeverity_Error                          = 5
} ELogSeverity;

#endif
