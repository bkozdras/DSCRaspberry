#pragma once

#include <boost/noncopyable.hpp>
#include "../SharedDefines/EMessageId.h"
#include "../Defines/CommonDefines.hpp"
#include <mutex>
#include <string>

class UartMessageMemoryManager : public boost::noncopyable
{
    public :

        static TByte* allocate(EMessageId messageId);
        static void deallocate(TByte* memoryPtr, EMessageId messageId);
        static u16 getLength(EMessageId messageId);

    private :

        static std::mutex mMtx;

        static const std::string & getLoggerPrefix();
};