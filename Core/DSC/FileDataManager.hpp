#pragma once

#include <boost/noncopyable.hpp>
#include <boost/filesystem.hpp>

#include <fstream>
#include <mutex>
#include <string>

#include "EControlMode.hpp"
#include "../SharedDefines/EUnitId.h"
#include "../System/TimerManager.hpp"
#include "DataManager.hpp"

namespace DSC
{
    class FileDataManager : public boost::noncopyable
    {
        public:

            static bool initialize();

            static void startRegisteringHeaterControlSystemData();
            static void stopRegisteringHeaterControlSystemData();

            static void startRegisteringDscData();
            static void stopRegisteringDscData();

        private:

            static void _startRegisteringHeaterControlSystemData();
            static void _stopRegisteringHeaterControlSystemData();

            static void _startRegisteringDscData();
            static void _stopRegisteringDscData();

            static void newAtributeCallback(EUnitId unitId, const std::string & attribute, const std::string & value);
            static void newControlModeCallback(EControlMode mode);

            static void storeHeaterControlSystemDataCallback();
            static void storeDscDataCallback();

            static bool createDataDirectoryIfDoesNotExist();
            static void ifFileExistsChangeName(const boost::filesystem::path & directoryPath, const std::string & fileNameAttribute);

            static bool openFile(std::ofstream & fileStream, const boost::filesystem::path & filePath);
            static bool closeFile(std::ofstream & fileStream);
            
            static void saveToFileCurrentData(std::ofstream & fileStream);
            static void saveToFileCurrentControlMode(std::ofstream & fileStream);
            static void saveToFileRegisteringPeriod(std::ofstream & fileStream);
            static void saveToFileDescribeData(std::ofstream & fileStream);

            static std::string getActualTime();
            static std::string getActualDate();
            static std::string getActualWeekday();

            static std::string convertDoubleToString(const double & value, int precision);

            static const std::string & getLoggerPrefix();

            static std::mutex mMtx;
            static std::ofstream mHeaterDataFileStream;
            static std::ofstream mDscDataFileStream;
            static boost::filesystem::path mHeaterControlSystemDataDirectoryPath;
            static boost::filesystem::path mDscDataDirectoryPath;
            static boost::filesystem::path mHeaterControlSystemDataFilePath;
            static boost::filesystem::path mDscDataFilePath;
            static unsigned long long mHeaterDataSampleNumber;
            static unsigned long long mDscDataSampleNumber;
            static EControlMode mControlMode;

            static DSC::DataManager::CallbackId mNewAttributeCallbackId;
            static bool mIsNewAttributeCallbackRegistered;
            static DSC::DataManager::CallbackId mNewControlModeCallbackId;
            static TimerManager::TimerId mHeaterControlSystemStoreDataCallbackId;
            static TimerManager::TimerId mDscStoreDataCallbackId;
    };
}