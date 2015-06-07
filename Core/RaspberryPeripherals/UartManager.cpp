#include "UartManager.hpp"
#include "../Utilities/Logger.hpp"
#include "../Utilities/ToStringConverter.hpp"
#include "../FaultManagement/FaultManager.hpp"
#include "../System/ThreadPool.hpp"
#include "UartMessageMemoryManager.hpp"

#include <boost/bind.hpp>
#include <termios.h>
#include <iostream>
#include <cstdio>
#include <unistd.h>	
#include <fcntl.h>

bool UartManager::initialize()
{
    Logger::debug("%s: Initialization started.", getLoggerPrefix().c_str());

    initializeInternalData();
    initializeTerminal();

    auto success = initializeHardwareConnection();

    if (success)
    {
        Logger::info("%s: Initialized!", getLoggerPrefix().c_str());
    }
    else
    {
        Logger::error("%s: Not initialized! Failure...", getLoggerPrefix().c_str());
    }

    return success;
}

void UartManager::transmitData(TMessage & message)
{
    Logger::debug("%s: Transmitting message: %s (Length: %u) to Nucleo device.", getLoggerPrefix().c_str(), ToStringConverter::getMessageId(message.id).c_str(), message.length);
    for (u8 iter = 0; message.length > iter; ++iter)
    {
        Logger::debug("%s: Message byte [%u]: 0x%02X.", getLoggerPrefix().c_str(), iter, message.data[iter]);
    }

    mAsyncSerial.write("MSG", 3);
    mAsyncSerial.write((char*) &(message.id), 1);
    mAsyncSerial.write((char*)&(message.transactionId), 1);

    TByte crcYounger = message.crc;
    TByte crcOlder = (message.crc >> 8) & 0xFF;
    mAsyncSerial.write((char*)&(crcYounger), 1);
    mAsyncSerial.write((char*)&(crcOlder), 1);

    mAsyncSerial.write((char*)&(message.length), 1);

    if (1U < message.length)
    {
        usleep(200000UL);
    }

    mAsyncSerial.write((char*)(message.data), message.length);

    //mAsyncSerial.write("END\n", 4);
}

void UartManager::registerCallbackForNewMessage(std::function<void(std::shared_ptr<TMessage>)>callback)
{
    mNewMessageCallback = callback;
    Logger::debug("%s: Register callback for new message.", getLoggerPrefix().c_str());
}

void UartManager::deregisterCallbackForNewMessage()
{
    mNewMessageCallback = decltype(mNewMessageCallback)();
    Logger::debug("%s: Unregistered callback for new message.", getLoggerPrefix().c_str());
}

void UartManager::initializeInternalData()
{
    mActualReceivingPhase = MessageReceivingPhase::Header;
    mReceivedDataBuffer.set_capacity(2048);
    mReceivedDataBuffer.clear();

    mHandlingMessage = std::make_shared<TMessage>();

    mNewMessageCallback = decltype(mNewMessageCallback)();

    mIsCommunicationFailureGenerated = false;
}

void UartManager::initializeTerminal()
{/*
    termios stored_settings;
    tcgetattr(0, &stored_settings);
    termios new_settings = stored_settings;
    new_settings.c_lflag &= (~ICANON);
    new_settings.c_lflag &= (~ISIG); // don't automatically handle control-C
    new_settings.c_lflag &= ~(ECHO); // no echo
    tcsetattr(0, TCSANOW, &new_settings);*/

    int USB = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY);

    struct termios tty;
    struct termios tty_old;
    memset(&tty, 0, sizeof tty);

    /* Error Handling */
    if (tcgetattr(USB, &tty) != 0) {
        std::cout << "Error " << errno << " from tcgetattr: " << strerror(errno) << std::endl;
    }

    /* Save old tty parameters */
    tty_old = tty;

    /* Set Baud Rate */
    cfsetospeed(&tty, (speed_t)B1000000);

    /* Setting other Port Stuff */
    tty.c_cflag &= ~PARENB;            // Make 8n1
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;

    tty.c_cflag &= ~CRTSCTS;           // no flow control
    tty.c_cc[VMIN] = 1;                  // read doesn't block
    tty.c_cc[VTIME] = 5;                  // 0.5 seconds read timeout
    tty.c_cflag |= CREAD | CLOCAL;     // turn on READ & ignore ctrl lines

    /* Make raw */
    cfmakeraw(&tty);

    /* Flush Port, then applies attributes */
    tcflush(USB, TCIFLUSH);
    if (tcsetattr(USB, TCSANOW, &tty) != 0) {
        std::cout << "Error " << errno << " from tcsetattr" << std::endl;
    }

    close(USB);

    Logger::debug("%s: Terminal options set.", getLoggerPrefix().c_str());
}

bool UartManager::initializeHardwareConnection()
{
    mAsyncSerial.open
    (
        "/dev/ttyAMA0",
        1000000
    );

    if (mAsyncSerial.errorStatus())
    {
        Logger::error("%s: Async Serial Port: Error Status!", getLoggerPrefix().c_str());
        FaultManager::generate(EFaultId_Uart, EUnitId_Raspberry, EUnitId_Empty);
        return false;
    }

    if (!mAsyncSerial.isOpen())
    {
        Logger::error("%s: Serial port is not open! Failure.", getLoggerPrefix().c_str());
        FaultManager::generate(EFaultId_Uart, EUnitId_Raspberry, EUnitId_Empty);
        return false;
    }

    mAsyncSerial.setCallback([](const char* data, size_t length){ newDataReceivedCallback(data, length); });

    Logger::debug("%s: Initialized hardware connection! Serial port is open with success.", getLoggerPrefix().c_str());
    Logger::debug("%s: Transmission with second device is now allowed.", getLoggerPrefix().c_str());

    return true;
}

bool UartManager::headerPhaseMessageParser()
{
    if (mReceivedDataBuffer.size() < 8)
    {
        return false;
    }

    if (!validateMessageHeader())
    {
        if (!mIsCommunicationFailureGenerated)
        {
            Logger::debug("%s: Message header verification failed!", getLoggerPrefix().c_str());
            //FaultManager::generate(EFaultId_Communication, EUnitId_Raspberry, EUnitId_Nucleo);
            mIsCommunicationFailureGenerated = true;
            Logger::debug("%s: Failure mode! Searching for correct data frame...", getLoggerPrefix().c_str());
            mActualReceivingPhase = MessageReceivingPhase::HeaderMsgFailureMode;
            //mAsyncSerial.clearCallback();
        }
        return false;
    }

    mHandlingMessage->id = static_cast<EMessageId>(getAndPopFromReceivedDataBuffer());
    mHandlingMessage->transactionId = getAndPopFromReceivedDataBuffer();
    mHandlingMessage->crc = getAndPopFromReceivedDataBuffer();
    mHandlingMessage->crc |= ((getAndPopFromReceivedDataBuffer() >> 8) && 0xFF);
    mHandlingMessage->length = getAndPopFromReceivedDataBuffer();
    mHandlingMessage->data = UartMessageMemoryManager::allocate(mHandlingMessage->id);

    mActualReceivingPhase = MessageReceivingPhase::Data;

    return true;
}

bool UartManager::dataPhaseMessageParser()
{
    if (mReceivedDataBuffer.size() < mHandlingMessage->length)
    {
        return false;
    }

    for (auto iter = 0; mHandlingMessage->length > iter; ++iter)
    {
        mHandlingMessage->data[iter] = getAndPopFromReceivedDataBuffer();
    }

    mActualReceivingPhase = MessageReceivingPhase::End;

    return true;
}

bool UartManager::endPhaseMessageParser()
{
    if (mReceivedDataBuffer.size() < 4)
    {
        return false;
    }

    mActualReceivingPhase = MessageReceivingPhase::Header;

    if (!validateMessageEnd())
    {
        if (!mIsCommunicationFailureGenerated)
        {
            Logger::debug("%s: Message end verification failed!", getLoggerPrefix().c_str());
            Logger::debug("%s: Receiving message: %s failed.", getLoggerPrefix().c_str(), ToStringConverter::getMessageId(mHandlingMessage->id).c_str());
            //FaultManager::generate(EFaultId_Communication, EUnitId_Raspberry, EUnitId_Nucleo);
            mIsCommunicationFailureGenerated = true;
            mActualReceivingPhase = MessageReceivingPhase::HeaderMsgFailureMode;
            Logger::debug("%s: Failure mode! Searching for correct data frame...", getLoggerPrefix().c_str());
            //mAsyncSerial.clearCallback();
        }
        return false;
    }

    processWithReceivedMessage();
    mHandlingMessage = std::make_shared<TMessage>();

    return true;
}

bool UartManager::headerMsgFailureModePhaseMessageParser()
{
    if (mReceivedDataBuffer.size() < 3)
    {
        return false;
    }

    static bool isMFound = false;
    static bool isSFound = false;
    bool isHeaderMsgFound = false;

    while (!mReceivedDataBuffer.empty() && !isHeaderMsgFound)
    {
        if (!isMFound)
        {
            if ('M' == getAndPopFromReceivedDataBuffer())
            {
                isMFound = true;
            }

            continue;
        }

        if (!isSFound)
        {
            if ('S' == getAndPopFromReceivedDataBuffer())
            {
                isSFound = true;
                continue;
            }

            isMFound = false;
            continue;
        }

        if ('G' == getAndPopFromReceivedDataBuffer())
        {
            isHeaderMsgFound = true;
        }
        
        isMFound = false;
        isSFound = false;
    }

    if (isHeaderMsgFound)
    {
        mActualReceivingPhase = MessageReceivingPhase::HeaderDataFailureMode;
        mIsCommunicationFailureGenerated = false;
        Logger::debug("%s: Communication established after incorrect data receiving!", getLoggerPrefix().c_str());
        //FaultManager::cancel(EFaultId_Communication, EUnitId_Raspberry, EUnitId_Nucleo);
    }

    return isHeaderMsgFound;
}

bool UartManager::headerDataFailureModePhaseMessageParser()
{
    if (mReceivedDataBuffer.size() < 5)
    {
        return false;
    }

    mHandlingMessage->id = static_cast<EMessageId>(getAndPopFromReceivedDataBuffer());
    mHandlingMessage->transactionId = getAndPopFromReceivedDataBuffer();
    mHandlingMessage->crc = getAndPopFromReceivedDataBuffer();
    mHandlingMessage->crc |= ((getAndPopFromReceivedDataBuffer() >> 8) && 0xFF);
    mHandlingMessage->length = getAndPopFromReceivedDataBuffer();
    mHandlingMessage->data = UartMessageMemoryManager::allocate(mHandlingMessage->id);

    mActualReceivingPhase = MessageReceivingPhase::Data;

    return true;
}

bool UartManager::validateMessageHeader()
{
    if ('M' != getAndPopFromReceivedDataBuffer())
    {
        return false;
    }

    if ('S' != getAndPopFromReceivedDataBuffer())
    {
        return false;
    }

    if ('G' != getAndPopFromReceivedDataBuffer())
    {
        return false;
    }

    return true;
}

bool UartManager::validateMessageEnd()
{
    std::vector<TByte> endMessage;
    endMessage.push_back(getAndPopFromReceivedDataBuffer());
    endMessage.push_back(getAndPopFromReceivedDataBuffer());
    endMessage.push_back(getAndPopFromReceivedDataBuffer());
    endMessage.push_back(getAndPopFromReceivedDataBuffer());

    if ('E' != endMessage[0])
    {
        for (const auto & end : endMessage)
        {
            Logger::debug("%s: Message End: 0x%02X (%c).", getLoggerPrefix().c_str(), end, end);
        }
        return false;
    }

    if ('N' != endMessage[1])
    {
        for (const auto & end : endMessage)
        {
            Logger::debug("%s: Message End: 0x%02X (%c).", getLoggerPrefix().c_str(), end, end);
        }
        return false;
    }

    if ('D' != endMessage[2])
    {
        for (const auto & end : endMessage)
        {
            Logger::debug("%s: Message End: 0x%02X (%c).", getLoggerPrefix().c_str(), end, end);
        }
        return false;
    }

    if ('\n' != endMessage[3])
    {
        for (const auto & end : endMessage)
        {
            Logger::debug("%s: Message End: 0x%02X (%c).", getLoggerPrefix().c_str(), end, end);
        }
        return false;
    }

    /*
    if ('E' != getAndPopFromReceivedDataBuffer())
    {
        return false;
    }

    if ('N' != getAndPopFromReceivedDataBuffer())
    {
        return false;
    }

    if ('D' != getAndPopFromReceivedDataBuffer())
    {
        return false;
    }

    if ('\n' != getAndPopFromReceivedDataBuffer())
    {
        return false;
    }*/

    return true;
}

void UartManager::newDataReceivedCallback(const char* data, size_t length)
{
    std::lock_guard<std::mutex> lockGuard(mReceivedDataMtx);

    mReceivedDataBuffer.insert(mReceivedDataBuffer.begin(), data, data + length);

    auto isParsingPassed = true;

    while (isParsingPassed)
    {
        switch (mActualReceivingPhase)
        {
            case MessageReceivingPhase::Header:
            {
                isParsingPassed = headerPhaseMessageParser();
                break;
            }

            case MessageReceivingPhase::Data:
            {
                isParsingPassed = dataPhaseMessageParser();
                break;
            }

            case MessageReceivingPhase::End:
            {
                isParsingPassed = endPhaseMessageParser();
                break;
            }

            case MessageReceivingPhase::HeaderMsgFailureMode:
            {
                isParsingPassed = headerMsgFailureModePhaseMessageParser();
                break;
            }

            case MessageReceivingPhase::HeaderDataFailureMode:
            {
                isParsingPassed = headerDataFailureModePhaseMessageParser();
                break;
            }
        }
    }
}

void UartManager::processWithReceivedMessage()
{
    if (mNewMessageCallback)
    {
        auto sharedHandlingMessage = mHandlingMessage;
        ThreadPool::submit
        (
            TaskPriority::High,
            [sharedHandlingMessage]()
            {
                mNewMessageCallback(sharedHandlingMessage);
            }
        );
    }
}

TByte UartManager::getAndPopFromReceivedDataBuffer()
{
    auto byte = mReceivedDataBuffer.front();
    mReceivedDataBuffer.pop_front();
    return byte;
}

const std::string & UartManager::getLoggerPrefix()
{
    static std::string loggerPrefix("UartManager");
    return loggerPrefix;
}

CallbackAsyncSerial UartManager::mAsyncSerial;
boost::circular_buffer<TByte> UartManager::mReceivedDataBuffer;
UartManager::MessageReceivingPhase UartManager::mActualReceivingPhase;
std::shared_ptr<TMessage> UartManager::mHandlingMessage;
std::mutex UartManager::mReceivedDataMtx;
std::function<void(std::shared_ptr<TMessage>)> UartManager::mNewMessageCallback;
bool UartManager::mIsCommunicationFailureGenerated;