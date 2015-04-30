#include "UartManager.hpp"
#include "../Utilities/Logger.hpp"
#include "../FaultManagement/FaultManager.hpp"
#include "../System/ThreadPool.hpp"
#include "UartMessageMemoryManager.hpp"

#include <boost/bind.hpp>
#include <termios.h>

bool UartManager::initialize()
{
    Logger::debug("%s: Initialization started.", getLoggerPrefix().c_str());

    initializeInternalData();
    initializeTerminal();

    bool success = true;

    Logger::info("%s: Initialized!", getLoggerPrefix().c_str());

    return success;
}

void UartManager::transmitData(TMessage & message)
{
    mAsyncSerial.write("MSG", 3);
    mAsyncSerial.write((char*) &(message.id), 1);
    mAsyncSerial.write((char*)&(message.transactionId), 1);

    TByte crcYounger = message.crc;
    TByte crcOlder = (message.crc >> 8) & 0xFF;
    mAsyncSerial.write((char*)&(crcYounger), 1);
    mAsyncSerial.write((char*)&(crcOlder), 1);

    mAsyncSerial.write((char*)&(message.length), 1);

    mAsyncSerial.write((char*)&(message.data), message.length);

    mAsyncSerial.write("END\n", 4);
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

    mAsyncSerial.setCallback([](const char* data, size_t length){ newDataReceivedCallback(data, length); });
}

void UartManager::initializeTerminal()
{
    termios stored_settings;
    tcgetattr(0, &stored_settings);
    termios new_settings = stored_settings;
    new_settings.c_lflag &= (~ICANON);
    new_settings.c_lflag &= (~ISIG); // don't automatically handle control-C
    new_settings.c_lflag &= ~(ECHO); // no echo
    tcsetattr(0, TCSANOW, &new_settings);

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
        Logger::error("%s: Message header verification failed!", getLoggerPrefix().c_str());
        FaultManager::generate(EFaultId_Communication, EUnitId_Raspberry, EUnitId_Nucleo);
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
}

bool UartManager::endPhaseMessageParser()
{
    if (mReceivedDataBuffer.size() < 4)
    {
        return false;
    }

    if (!validateMessageEnd())
    {
        Logger::error("%s: Message end verification failed!", getLoggerPrefix().c_str());
        FaultManager::generate(EFaultId_Communication, EUnitId_Raspberry, EUnitId_Nucleo);
        return false;
    }

    processWithReceivedMessage();
    mHandlingMessage = std::make_shared<TMessage>();

    mActualReceivingPhase = MessageReceivingPhase::Header;

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
    }

    return true;
}

void UartManager::newDataReceivedCallback(const char* data, size_t length)
{
    std::lock_guard<std::mutex> lockGuard(mReceivedDataMtx);

    mReceivedDataBuffer.insert(mReceivedDataBuffer.begin(), data, data + length);

    while (!mReceivedDataBuffer.empty())
    {
        switch (mActualReceivingPhase)
        {
            case MessageReceivingPhase::Header:
            {
                headerPhaseMessageParser();
                break;
            }

            case MessageReceivingPhase::Data:
            {
                dataPhaseMessageParser();
                break;
            }

            case MessageReceivingPhase::End:
            {
                endPhaseMessageParser();
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