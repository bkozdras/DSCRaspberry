#pragma once

#include <boost/noncopyable.hpp>
#include <mutex>
#include <string>
#include <vector>

#include "../SharedDefines/MessagesDefines.h"
#include "../SharedDefines/SSegmentData.h"
#include "../Defines/CommonDefines.hpp"

namespace DSC
{
    class SegmentsManager : public boost::noncopyable
    {
        public :

            enum class TimeUnit : u8
            {
                Seconds,
                Minutes,
                Hours
            };

            struct SegmentData
            {
                SSegmentData nucleoData;
                TimeUnit programDurationUnit;
                double programDuration;
            };

            static bool initialize();
            
            static u8 saveSegment(ESegmentType type, double startTemperature, double stopTemperature, double temperatureStep, double programDuration, TimeUnit programDurationUnit);
            static void deleteSegment(u8 number);

            static bool registerAllSegments();
            static bool startProgram();
            static bool stopProgram();
            static const SegmentData & getSegmentData(u8 number);
            static u8 getNumberOfSegments();
            static u8 getNextFreeSegmentNumber();

        private :

            static void registerNewSegmentToProgramResponseCallback(TRegisterNewSegmentToProgramResponse && response);
            static void startSegmentProgramResponseCallback(TStartSegmentProgramResponse && response);
            static void stopSegmentProgramResponseCallback(TStopSegmentProgramResponse && response);
            static void segmentStartedIndCallback(TSegmentStartedInd && ind);
            static void segmentProgramDoneIndCallback(TSegmentsProgramDoneInd && ind);


            static u8 generateNextSegmentNumber();
            static double convertTimeToMilliseconds(TimeUnit unit, double value);

            static const std::string & getLoggerPrefix();

            static std::mutex mMtx;
            static std::vector<SegmentData> mSegments;
            static u8 mNextFreeSegmentNumber;
            static u8 mRegisteredSegmentsCount;
            static bool mIsSegmentsRegistered;
    };
}