#pragma once

#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include <boost/noncopyable.hpp>

#include "LoggerTypes.hpp"
#include "../SharedDefines/MessagesDefines.h"
#include "../System/TimerManager.hpp"

class Logger : boost::noncopyable
{
    public :

        enum class LogSource
        {
            Raspberry,
            Nucleo
        };

        struct LogTimeData
        {
            std::string data;
            std::string weakDay;
            std::string time;
        };

        struct StoredLog
        {
            ELogSeverity severity;
            LogSource source;
            LogTimeData timeData;
            std::string log;
        };

    private :

        struct LoggerOutputData
        {
            LoggerLevel level;
            std::vector<std::shared_ptr<StoredLog>> storedLogs;
            TimerManager::TimerId timerId;
            u16 updateTimeInterval;
            std::function<void()> updateCallback;
        };

    public :

        static void initialize();

        static void setLevel(LoggerOutput output, LoggerLevel level);
        static void setLevel(LoggerLevel level);

        static void debug(const std::string & format, ...);
        static void info(const std::string & format, ...);
        static void warning(const std::string & format, ...);
        static void error(const std::string & format, ...);

        static void nucleoInd(std::shared_ptr<TLogInd> logInd);

        static void registerGuiTableLogCallback(std::function<void(std::shared_ptr<Logger::StoredLog>)> callback);
        static void deregisterGuiTableLogCallback();

    private :

        Logger() = delete;

        static std::mutex mDataMtx;
        static std::mutex mStoredLogsGuiTableMtx;
        static std::mutex mStoredLogsSnapshotFileMtx;
        static std::mutex mSnapshotFileMtx;
        static std::map<LoggerOutput, LoggerOutputData> mLoggerOutputsData;
        static std::function<void(std::shared_ptr<Logger::StoredLog>)> mGuiTableLogCallback;

        static bool isLogShouldBePrinted(ELogSeverity severity);
        static void storeLog(ELogSeverity severity, const std::string & format, va_list vaList);
        static void storeLog(std::shared_ptr<StoredLog> && log);
        static void fillWithActualTime(std::shared_ptr<StoredLog> & log);

        static void updateLoggerTimerStates();
        static void updateGuiTable();
        static void updateSnapshotFile();
};