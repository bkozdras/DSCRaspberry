#pragma once

#include <boost/noncopyable.hpp>
#include <string>
#include <memory>
#include "../SharedDefines/SFaultIndication.h"

class SystemErrorsManager : public boost::noncopyable
{
    public :

        static bool initialize();

    private :

        static void printStacktrace();
        static void signalHandler(int sig);
        static void terminateHandler();

		static void newFaultHandler(std::shared_ptr<SFaultIndication> faultPtr);

        static const std::string & getLoggerPrefix();
};