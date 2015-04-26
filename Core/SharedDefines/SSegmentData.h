#ifndef _S_SEGMENT_DATA_H_

#define _S_SEGMENT_DATA_H_

#ifdef RASPBERRY

#include "../Defines/CommonDefines.hpp"

#else

#include "Defines/CommonDefines.h"

#endif

typedef enum _ESegmentType
{
    ESegmentType_Static     = 0,
    ESegmentType_Dynamic    = 1
} ESegmentType;

typedef struct _SSegmentData
{
    u16 number;
    ESegmentType type;
    float startTemperature;
    float stopTemperature;
    u32 settingTimeInterval;
    float temperatureStep; /* for dynamic segments only */
} SSegmentData;

#endif
