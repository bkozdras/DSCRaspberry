#include "SystemErrorsManager.hpp"

#include "../Utilities/Logger.hpp"
#include "../FaultManagement/FaultManager.hpp"
#include "../ModelIdentification/ExperimentManager.hpp"
#include "../DevicePeripherals/StateManager.hpp"

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
        std::fprintf(stderr, "%s: Backtrace(%u): %s.\n", getLoggerPrefix().c_str(), functionSymbol, backtraceSymbols[functionSymbol]);
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
            std::fprintf(stderr, "Terminated due to unknown exception!\n");
            //Logger::error("%s: Terminated due to unknown exception.", getLoggerPrefix().c_str());
        }
    }
    else
    {
        std::fprintf(stderr, "Terminated due to unknown reason!\n");
        //Logger::error("%s: Terminated due to unknown reason.", getLoggerPrefix().c_str());
    }
    printStacktrace();
    std::abort();
}

void SystemErrorsManager::newFaultHandler(std::shared_ptr<SFaultIndication> faultPtr)
{
	if (EFaultId_Uart == faultPtr->faultId && EUnitId_Nucleo == faultPtr->faultyUnitId)
	{
		Logger::error("%s: Detected failures in UART communication with Slave.", getLoggerPrefix().c_str());
		if (EExperimentState::Running == ModelIdentification::ExperimentManager::getExperimentState())
		{
			Logger::error("%s: Model Identification experiment is running. Turning off...", getLoggerPrefix().c_str());
			ModelIdentification::ExperimentManager::forceStoppingExperiment();
		}
		if (DevicePeripherals::State::PowerOn == DevicePeripherals::StateManager::getState())
		{
			Logger::error("%s: Disabling Slave device...", getLoggerPrefix().c_str());
			DevicePeripherals::StateManager::turnPowerOff();
		}
	}
}

bool SystemErrorsManager::initialize()
{
    Logger::debug("%s: Initializing.");

    signal(SIGSEGV, &SystemErrorsManager::signalHandler);
    std::set_terminate(terminateHandler);
	FaultManager::registerNotificationCallback([](std::shared_ptr<SFaultIndication> faultInd) {
		newFaultHandler(faultInd);
	});

    Logger::info("%s: Initialized!", getLoggerPrefix().c_str());

    return true;
}

const std::string & SystemErrorsManager::getLoggerPrefix()
{
    static std::string loggerPrefix("SystemErrorsManager");
    return loggerPrefix;
}