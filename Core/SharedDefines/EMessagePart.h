#ifndef _E_MESSAGE_PART_H_

#define _E_MESSAGE_PART_H_

#include "../Defines/CommonDefines.hpp"

typedef enum _EMessagePart : u8
{
    EMessagePart_Header         = 0,
    EMessagePart_Data           = 1,
    EMessagePart_End            = 2
} EMessagePart;

#endif
