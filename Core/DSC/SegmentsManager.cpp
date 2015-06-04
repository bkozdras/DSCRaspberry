#include "SegmentsManager.hpp"
#include "DataManager.hpp"
#include "../Utilities/Logger.hpp"
#include "../Utilities/ToStringConverter.hpp"
#include "../Nucleo/DeviceCommunicator.hpp"

#include <algorithm>

namespace DSC
{
    bool SegmentsManager::initialize()
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        mSegments.clear();
        mNextFreeSegmentNumber = generateNextSegmentNumber();
        mRegisteredSegmentsCount = 0;
        mIsSegmentsRegistered = false;

        DataManager::updateData(EDataType::ActualRealizedSegment, 0.0);
        DataManager::updateData(EDataType::IsSegmentsProgramOngoing, 0.0);
        DataManager::updateData(EDataType::NumberOfRegisteredSegments, 0.0);

        Logger::info("%s: Initialized!", getLoggerPrefix().c_str());
        return true;
    }

    u8 SegmentsManager::saveSegment(ESegmentType type, float startTemperature, float stopTemperature, u32 time, float temperatureStep)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);

        SSegmentData data;
        data.number = mNextFreeSegmentNumber;
        data.settingTimeInterval = time;
        data.startTemperature = startTemperature;
        data.stopTemperature = stopTemperature;
        data.temperatureStep = temperatureStep;
        data.type = type;

        mSegments.emplace_back(std::move(data));

        mNextFreeSegmentNumber = generateNextSegmentNumber();

        Logger::info("%s: Saved new segment (Number: %u). Type: %s.", getLoggerPrefix().c_str(), ToStringConverter::getSegmentType(type).c_str());
    }

    void SegmentsManager::deleteSegment(u8 number)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);

        std::remove_if
        (
            std::begin(mSegments),
            std::end(mSegments),
            [number](const SSegmentData & data)
            {
                if (data.number == number)
                {
                    return true;
                }

                return false;
            }
        );
    }

    bool SegmentsManager::registerAllSegments()
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);

        mRegisteredSegmentsCount = 0;

        if (mSegments.empty())
        {
            Logger::warning("%s: No segments to register!", getLoggerPrefix().c_str());
            return false;
        }

        Logger::info("%s: Registering saved segments to Nucleo device...", getLoggerPrefix().c_str());

        TRegisterNewSegmentToProgramRequest request;
        request.segment = mSegments.front();
        Logger::info("%s: Registering segment number: %u/%u.", getLoggerPrefix().c_str(), request.segment.number, mSegments.size());
        Nucleo::DeviceCommunicator::send(request, [](TRegisterNewSegmentToProgramResponse && response, bool isNotTimeout){ registerNewSegmentToProgramResponseCallback(std::move(response)); });

        return true;
    }

    bool SegmentsManager::startProgram()
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);

        if (!mIsSegmentsRegistered)
        {
            Logger::warning("%s: Segments are not registered to Nucleo!", getLoggerPrefix().c_str());
            return false;
        }

        Logger::info("%s: Starting segments program...", getLoggerPrefix().c_str());

        TStartSegmentProgramRequest request;
        Nucleo::DeviceCommunicator::send(request, [](TStartSegmentProgramResponse && response, bool isNotTimeout){ startSegmentProgramResponseCallback(std::move(response)); });

        return true;
    }

    bool SegmentsManager::stopProgram()
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);

        if (!mIsSegmentsRegistered)
        {
            Logger::warning("%s: Segments are not registered to Nucleo!", getLoggerPrefix().c_str());
            return false;
        }

        Logger::info("%s: Stopping segments program...", getLoggerPrefix().c_str());

        TStopSegmentProgramRequest request;
        Nucleo::DeviceCommunicator::send(request, [](TStopSegmentProgramResponse && response, bool isNotTimeout){ stopSegmentProgramResponseCallback(std::move(response)); });

        return true;
    }

    const SSegmentData & SegmentsManager::getSegmentData(u8 number)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        return mSegments.at(number);
    }

    u8 SegmentsManager::getNumberOfSegments()
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        return mSegments.size();
    }

    void SegmentsManager::registerNewSegmentToProgramResponseCallback(TRegisterNewSegmentToProgramResponse && response)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);

        if (response.success)
        {
            Logger::info("%s: Registered segment number: %u.", getLoggerPrefix().c_str(), response.segmentNumber);
            Logger::info("%s: Registered segments count: %u.", getLoggerPrefix().c_str(), response.numberOfRegisteredSegments);
            mRegisteredSegmentsCount++;
            DSC::DataManager::updateData(EDataType::NumberOfRegisteredSegments, mRegisteredSegmentsCount);

            if (mSegments.size() != mRegisteredSegmentsCount)
            {
                TRegisterNewSegmentToProgramRequest request;
                request.segment = mSegments.at(mRegisteredSegmentsCount);
                Logger::info("%s: Registering segment number: %u/%u.", getLoggerPrefix().c_str(), request.segment.number, mSegments.size());
                Nucleo::DeviceCommunicator::send(request, [](TRegisterNewSegmentToProgramResponse && response, bool isNotTimeout){ registerNewSegmentToProgramResponseCallback(std::move(response)); });
            }
            else
            {
                mIsSegmentsRegistered = true;
                Logger::info("%s: All segments registered correctly. Waiting for program starting...", getLoggerPrefix().c_str());
            }
        }
        else
        {
            Logger::error("%s: Registering segment number: %u failed!", getLoggerPrefix().c_str(), response.segmentNumber);
        }
    }

    void SegmentsManager::startSegmentProgramResponseCallback(TStartSegmentProgramResponse && response)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);

        if (response.success)
        {
            Logger::info("%s: Segments program started!", getLoggerPrefix().c_str());
            DataManager::updateData(EDataType::IsSegmentsProgramOngoing, 1.0);
        }
        else
        {
            Logger::error("%s: Starting segments program failed!", getLoggerPrefix().c_str());
        }
    }

    void SegmentsManager::stopSegmentProgramResponseCallback(TStopSegmentProgramResponse && response)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);

        if (response.success)
        {
            Logger::info("%s: Segments program stopped!", getLoggerPrefix().c_str());
            DataManager::updateData(EDataType::IsSegmentsProgramOngoing, 0.0);
        }
        else
        {
            Logger::error("%s: Starting segments program failed!", getLoggerPrefix().c_str());
        }
    }

    void SegmentsManager::segmentStartedIndCallback(TSegmentStartedInd && ind)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);

        Logger::info("%s: Segment %u started (%u segments wait in queue)!", getLoggerPrefix().c_str(), ind.segmentNumber, ind.leftRegisteredSegments);
        DataManager::updateData(EDataType::ActualRealizedSegment, ind.segmentNumber);
    }

    void SegmentsManager::segmentProgramDoneIndCallback(TSegmentsProgramDoneInd && ind)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);

        Logger::info("%s: Segments program done!", getLoggerPrefix().c_str());
        DataManager::updateData(EDataType::ActualRealizedSegment, 0.0);
        DataManager::updateData(EDataType::IsSegmentsProgramOngoing, 0.0);
    }

    u8 SegmentsManager::generateNextSegmentNumber()
    {
        static u8 counter = 0;
        return ++counter;
    }

    const std::string & SegmentsManager::getLoggerPrefix()
    {
        static std::string loggerPrefix("SegmentsManager");
        return loggerPrefix;
    }

    std::mutex SegmentsManager::mMtx;
    std::vector<SSegmentData> SegmentsManager::mSegments;
    u8 SegmentsManager::mNextFreeSegmentNumber;
    u8 SegmentsManager::mRegisteredSegmentsCount;
    bool SegmentsManager::mIsSegmentsRegistered;
}