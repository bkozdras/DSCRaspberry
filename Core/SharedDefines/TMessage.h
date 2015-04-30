#ifndef _T_MESSAGE_H_

#define _T_MESSAGE_H_

#ifdef RASPBERRY

    #include "../Defines/CommonDefines.hpp"
    #include "EMessageId.h"

#else

    #include "Defines/CommonDefines.h"
    #include "SharedDefines/EMessageId.h"

#endif

typedef struct _TMessage
{
    EMessageId id;
    u8 transactionId;
    u16 crc;
    u8 length;
    TByte* data;
} TMessage;

#endif
