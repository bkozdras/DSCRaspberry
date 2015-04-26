#pragma once

#include "AsyncSerial.hpp"
#include <boost/circular_buffer.hpp>
#include <boost/noncopyable.hpp>
#include <string>

class UartManager : public boost::noncopyable
{
    public :

        static bool initialize();
        static bool transmitData(const char* data, size_t size);
        static void registerCallbackForReceivedData();

    private :

        enum class MessageReceivingPhase
        {
            Header,
            Data,
            End
        };

        static CallbackAsyncSerial mAsyncSerial;
        static boost::circular_buffer<char> mReceivedDataBuffer;
        static MessageReceivingPhase mActualReceivingPhase;

        static void initializeInternalData();
        static void initializeTerminal();

        static void newDataReceivedCallback(const char* data, size_t length);

        static const std::string & getLoggerPrefix();
};