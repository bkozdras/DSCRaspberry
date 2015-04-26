#include "Logger.hpp"

#include <cstdarg>
#include <cstdio>

#include <algorithm>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <locale>
#include <sstream>

#include "../System/ThreadPool.hpp"

void Logger::initialize()
{
    std::lock_guard<std::mutex> lockGuard(mDataMtx);

    mLoggerOutputsData = decltype(mLoggerOutputsData)
    {
        { LoggerOutput::GuiTable, { LoggerLevel::Off, std::vector<std::shared_ptr<StoredLog>>(), 0U, 500U, std::bind(&Logger::updateGuiTable) } },
        { LoggerOutput::SnapshotFile, { LoggerLevel::Off, std::vector<std::shared_ptr<StoredLog>>(), 0U, 2000U, std::bind(&Logger::updateSnapshotFile) } }
    };
}

void Logger::setLevel(LoggerOutput output, LoggerLevel level)
{
    std::lock_guard<std::mutex> lockGuard(mDataMtx);

    mLoggerOutputsData[output].level = level;
    updateLoggerTimerStates();
}

void Logger::setLevel(LoggerLevel level)
{
    std::lock_guard<std::mutex> lockGuard(mDataMtx);
    for (auto & loggerOutputData : mLoggerOutputsData)
    {
        loggerOutputData.second.level = level;
    }

    updateLoggerTimerStates();
}

void Logger::debug(const std::string & format, ...)
{
    std::lock_guard<std::mutex> lockGuard(mDataMtx);

    if (!isLogShouldBePrinted(ELogSeverity_Debug))
    {
        return;
    }

    va_list vaList;
    va_start(vaList, format);
    storeLog(ELogSeverity_Debug, format, vaList);
    va_end(vaList);
}

void Logger::info(const std::string & format, ...)
{
    std::lock_guard<std::mutex> lockGuard(mDataMtx);

    if (!isLogShouldBePrinted(ELogSeverity_Info))
    {
        return;
    }

    va_list vaList;
    va_start(vaList, format);
    storeLog(ELogSeverity_Info, format, vaList);
    va_end(vaList);
}

void Logger::warning(const std::string & format, ...)
{
    std::lock_guard<std::mutex> lockGuard(mDataMtx);

    if (!isLogShouldBePrinted(ELogSeverity_Warning))
    {
        return;
    }

    va_list vaList;
    va_start(vaList, format);
    storeLog(ELogSeverity_Warning, format, vaList);
    va_end(vaList);
}

void Logger::error(const std::string & format, ...)
{
    std::lock_guard<std::mutex> lockGuard(mDataMtx);

    if (!isLogShouldBePrinted(ELogSeverity_Error))
    {
        return;
    }

    va_list vaList;
    va_start(vaList, format);
    storeLog(ELogSeverity_Error, format, vaList);
    va_end(vaList);
}

void Logger::nucleoInd(std::shared_ptr<TLogInd> logInd)
{
    std::lock_guard<std::mutex> lockGuard(mDataMtx);

    auto storedLog = std::make_shared<StoredLog>();
    storedLog->log = logInd->data;
    storedLog->severity = logInd->severity;
    storedLog->source = LogSource::Nucleo;
    fillWithActualTime(storedLog);

    storeLog(std::move(storedLog));
}

void Logger::registerGuiTableLogCallback(std::function<void(std::shared_ptr<Logger::StoredLog>)> callback)
{
    std::lock_guard<std::mutex> lockGuard(mDataMtx);
    mGuiTableLogCallback = callback;
}

void Logger::deregisterGuiTableLogCallback()
{
    std::lock_guard<std::mutex> lockGuard(mDataMtx);
    mGuiTableLogCallback = decltype(mGuiTableLogCallback)();
}

bool Logger::isLogShouldBePrinted(ELogSeverity severity)
{
    bool shouldBePrinted = false;
    bool isAtLeastOneDebugLevelActive = false;

    for (const auto & loggerOutputData : mLoggerOutputsData)
    {
        if (LoggerLevel::Off != loggerOutputData.second.level)
        {
            shouldBePrinted = true;
            
            if (LoggerLevel::Debug == loggerOutputData.second.level)
            {
                isAtLeastOneDebugLevelActive = true;
            }
        }
    }

    if (ELogSeverity_Debug == severity && !isAtLeastOneDebugLevelActive)
    {
        shouldBePrinted = false;
    }

    return shouldBePrinted;
}

void Logger::storeLog(ELogSeverity severity, const std::string & format, va_list vaList)
{
    auto storedLog = std::make_shared<StoredLog>();
    storedLog->log.resize(220);
    vsprintf(const_cast<char*>(storedLog->log.c_str()), format.c_str(), vaList );
    storedLog->log.shrink_to_fit();

    storedLog->severity = severity;
    storedLog->source = LogSource::Raspberry;
    fillWithActualTime(storedLog);

    storeLog(std::move(storedLog));
}

void Logger::storeLog(std::shared_ptr<StoredLog> && log)
{
    for (auto & loggerOutputData : mLoggerOutputsData)
    {
        if (LoggerLevel::Off != loggerOutputData.second.level)
        {
            switch (loggerOutputData.second.level)
            {
                case LoggerLevel::Info:
                {
                    if (ELogSeverity_Debug == log->severity)
                    {
                        break;
                    }
                }

                default:
                {
                    if (LoggerOutput::GuiTable == loggerOutputData.first)
                    {
                        std::lock_guard<std::mutex> lockGuard(mStoredLogsGuiTableMtx);
                        loggerOutputData.second.storedLogs.push_back(log);
                    }
                    else if (LoggerOutput::SnapshotFile == loggerOutputData.first)
                    {
                        std::lock_guard<std::mutex> lockGuard(mStoredLogsSnapshotFileMtx);
                        loggerOutputData.second.storedLogs.push_back(log);
                    }
                    break;
                }
            }
        }
    }
}

void Logger::fillWithActualTime(std::shared_ptr<StoredLog> & log)
{
    auto actualTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    auto localTime = std::localtime(&actualTime);

    std::locale locale;
    const auto & tempPut = std::use_facet<std::time_put<char>>(locale);
    std::string timePattern;
    std::stringstream timeStream;

    // Data
    timePattern = "%F";
    tempPut.put(timeStream, timeStream, ' ', localTime, timePattern.c_str(), timePattern.c_str() + timePattern.length());
    log->timeData.data = timeStream.str();
    timeStream.str("");

    // Weakday
    timePattern = "%A";
    tempPut.put(timeStream, timeStream, ' ', localTime, timePattern.c_str(), timePattern.c_str() + timePattern.length());
    log->timeData.weakDay = timeStream.str();
    timeStream.str("");

    // Time
    timePattern = "%T";
    tempPut.put(timeStream, timeStream, ' ', localTime, timePattern.c_str(), timePattern.c_str() + timePattern.length());
    log->timeData.time = timeStream.str();
}

void Logger::updateLoggerTimerStates()
{
    for (auto & loggerOutputData : mLoggerOutputsData)
    {
        if (LoggerLevel::Off == loggerOutputData.second.level && 0 != loggerOutputData.second.timerId)
        {
            TimerManager::destroy(loggerOutputData.second.timerId);
            loggerOutputData.second.timerId = 0;
        }

        if (LoggerLevel::Off != loggerOutputData.second.level && 0 == loggerOutputData.second.timerId)
        {
            loggerOutputData.second.timerId = TimerManager::create
            (
                0,
                loggerOutputData.second.updateTimeInterval,
                [&loggerOutputData]()
                {
                    ThreadPool::submit(TaskPriority::Low, loggerOutputData.second.updateCallback);
                }
            );
        }
    }
}

void Logger::updateGuiTable()
{
    LoggerOutputData* loggerOutputData = nullptr;
    {
        std::lock_guard<std::mutex> lockGuard(mDataMtx);
        loggerOutputData = &(mLoggerOutputsData[LoggerOutput::GuiTable]);
    }

    std::lock_guard<std::mutex> lockGuard(mStoredLogsGuiTableMtx);
    for (const auto & storedLog : loggerOutputData->storedLogs)
    {
        if (mGuiTableLogCallback)
        {
            mGuiTableLogCallback(storedLog);
        }
    }
    loggerOutputData->storedLogs.clear();
}

void Logger::updateSnapshotFile()
{

}

std::mutex Logger::mDataMtx;
std::mutex Logger::mStoredLogsGuiTableMtx;
std::mutex Logger::mStoredLogsSnapshotFileMtx;
std::mutex Logger::mSnapshotFileMtx;
std::map<LoggerOutput, Logger::LoggerOutputData> Logger::mLoggerOutputsData;
std::function<void(std::shared_ptr<Logger::StoredLog>)> Logger::mGuiTableLogCallback;
