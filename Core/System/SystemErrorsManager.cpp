#include "SystemErrorsManager.hpp"

#include "../Utilities/Logger.hpp"

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <exception>
#include <stdexcept>
#include <execinfo.h>
#include <signal.h>
#include <unistd.h>

void SystemErrorsManager::printStacktrace()
{
    void *array[40];
    size_t size = backtrace(array, sizeof(array) / sizeof(array[0]));
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    auto backtraceSymbols = backtrace_symbols(array, size);
    for (size_t functionSymbol = 0; size > functionSymbol; ++functionSymbol)
    {
        //Logger::error("%s: Backtrace(%u): %s.", getLoggerPrefix().c_str(), functionSymbol, backtraceSymbols[functionSymbol]);
    }
}

void SystemErrorsManager::signalHandler(int sig)
{
    std::fprintf(stderr, "Signal %d caught!\n", sig);
    //Logger::error("%s: Signal %d caught!", getLoggerPrefix().c_str(), sig);
    printStacktrace();
    std::abort();
}

void SystemErrorsManager::terminateHandler()
{
    std::exception_ptr exptr = std::current_exception();
    if (exptr != 0)
    {
        // the only useful feature of std::exception_ptr is that it can be rethrown...
        try
        {
            std::rethrow_exception(exptr);
        }
        catch (std::exception &ex)
        {
            std::fprintf(stderr, "Terminated due to exception %s.\n", ex.what());
            //Logger::error("%s: Terminated due to exception: %s.", getLoggerPrefix().c_str(), ex.what());
        }
        catch (...)
        {
            std::fprintf(stderr, "Terminated due to unknown exception!n");
            //Logger::error("%s: Terminated due to unknown exception.", getLoggerPrefix().c_str());
        }
    }
    else
    {
        std::fprintf(stderr, "Terminated due to unknown reason!n");
        //Logger::error("%s: Terminated due to unknown reason.", getLoggerPrefix().c_str());
    }
    printStacktrace();
    std::abort();
}

bool SystemErrorsManager::initialize()
{
    Logger::debug("%s: Initializing.");

    signal(SIGSEGV, &SystemErrorsManager::signalHandler);
    std::set_terminate(terminateHandler);

    Logger::info("%s: Initialized!", getLoggerPrefix().c_str());

    return true;
}

const std::string & SystemErrorsManager::getLoggerPrefix()
{
    static std::string loggerPrefix("SystemErrorsManager");
    return loggerPrefix;
}