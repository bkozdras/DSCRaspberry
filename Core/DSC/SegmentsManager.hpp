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

            static bool initialize();
            
            static u8 saveSegment(ESegmentType type, float startTemperature, float stopTemperature, u32 time, float temperatureStep = 0.0F);
            static void deleteSegment(u8 number);

            static bool registerAllSegments();
            static bool startProgram();
            static bool stopProgram();
            static const SSegmentData & getSegmentData(u8 number);
            static u8 getNumberOfSegments();

        private :

            static void registerNewSegmentToProgramResponseCallback(TRegisterNewSegmentToProgramResponse && response);
            static void startSegmentProgramResponseCallback(TStartSegmentProgramResponse && response);
            static void stopSegmentProgramResponseCallback(TStopSegmentProgramResponse && response);
            static void segmentStartedIndCallback(TSegmentStartedInd && ind);
            static void segmentProgramDoneIndCallback(TSegmentsProgramDoneInd && ind);

            static u8 generateNextSegmentNumber();

            static const std::string & getLoggerPrefix();

            static std::mutex mMtx;
            static std::vector<SSegmentData> mSegments;
            static u8 mNextFreeSegmentNumber;
            static u8 mRegisteredSegmentsCount;
            static bool mIsSegmentsRegistered;
    };
}