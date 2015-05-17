#pragma once

#include <boost/noncopyable.hpp>
#include <string>

class SystemErrorsManager : public boost::noncopyable
{
    public :

        static bool initialize();

    private :

        static void printStacktrace();
        static void signalHandler(int sig);
        static void terminateHandler();

        static const std::string & getLoggerPrefix();
};