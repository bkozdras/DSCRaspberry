#pragma once

#include <boost/noncopyable.hpp>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "../SharedDefines/EMessageId.h"
#include "../SharedDefines/TMessage.h"
#include "../Defines/CommonDefines.hpp"

namespace Nucleo
{
    class MessageGateway : public boost::noncopyable
    {
        public :

            static void transmit(EMessageId messageId, TByte* data);
            static void registerNewMessageCallback(EMessageId messageId, std::function<void(std::shared_ptr<TMessage>)> callback);
            static void deregisterNewMessageCallback(EMessageId messageId);

        private :

            static std::map<EMessageId, std::function<void(std::shared_ptr<TMessage>)>> mNewDataCallbacks;
            static std::mutex mMtx;

            void newMessageCallback(std::shared_ptr<TMessage> message);
            const std::string & getLoggerPrefix();
    };
}