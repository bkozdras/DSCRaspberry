#pragma once

#include "AsyncSerial.hpp"
#include "../SharedDefines/TMessage.h"
#include "../Defines/CommonDefines.hpp"
#include <boost/circular_buffer.hpp>
#include <boost/noncopyable.hpp>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

class UartManager : public boost::noncopyable
{
    public :

        static bool initialize();
        static void transmitData(TMessage & message);
        static void registerCallbackForNewMessage(std::function<void(std::shared_ptr<TMessage>)> callback);
        static void deregisterCallbackForNewMessage();

    private :

        enum class MessageReceivingPhase
        {
            Header,
            Data,
            End
        };

        static CallbackAsyncSerial mAsyncSerial;
        static boost::circular_buffer<TByte> mReceivedDataBuffer;
        static MessageReceivingPhase mActualReceivingPhase;
        static std::shared_ptr<TMessage> mHandlingMessage;
        static std::mutex mReceivedDataMtx;
        static std::function<void(std::shared_ptr<TMessage>)> mNewMessageCallback;
        static bool mIsCommunicationFailureGenerated;
        static u8 mWrongMessagesCount;

        static void initializeInternalData();
        static void initializeTerminal();
        static bool initializeHardwareConnection();

        static bool headerPhaseMessageParser();
        static bool dataPhaseMessageParser();
        static bool endPhaseMessageParser();

        static bool validateMessageHeader();
        static bool validateMessageEnd();

        static void newDataReceivedCallback(const char* data, size_t length);
        static void processWithReceivedMessage();

        static TByte getAndPopFromReceivedDataBuffer();

        static const std::string & getLoggerPrefix();
};