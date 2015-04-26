#include "UartManager.hpp"
#include "../Utilities/Logger.hpp"

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

bool UartManager::transmitData(const char* data, size_t size)
{
    return false;
}

void UartManager::registerCallbackForReceivedData()
{

}

void UartManager::initializeInternalData()
{
    mActualReceivingPhase = MessageReceivingPhase::Header;
    mReceivedDataBuffer.set_capacity(2048);

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

void UartManager::newDataReceivedCallback(const char* data, size_t length)
{

}

const std::string & UartManager::getLoggerPrefix()
{
    static std::string loggerPrefix("UartManager");
    return loggerPrefix;
}

CallbackAsyncSerial UartManager::mAsyncSerial;
boost::circular_buffer<char> UartManager::mReceivedDataBuffer;
UartManager::MessageReceivingPhase UartManager::mActualReceivingPhase;