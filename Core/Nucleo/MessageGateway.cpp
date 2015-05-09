#include "MessageGateway.hpp"
#include "../RaspberryPeripherals/UartMessageMemoryManager.hpp"
#include "../RaspberryPeripherals/UartManager.hpp"

namespace Nucleo
{
    void MessageGateway::transmit(EMessageId messageId, TByte* data)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);

        TMessage message;
        message.crc = 0;
        message.data = data;
        message.id = messageId;
        message.length = UartMessageMemoryManager::getLength(messageId);
        UartManager::transmitData(message);
    }

    const std::string & MessageGateway::getLoggerPrefix()
    {
        static const std::string loggerPrefix("MessageGateway");
        return loggerPrefix;
    }

    std::mutex MessageGateway::mMtx;
}