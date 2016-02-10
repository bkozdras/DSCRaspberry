#pragma once

#include <fstream>
#include <mutex>
#include <queue>
#include <string>

#include <boost/noncopyable.hpp>

#include "../Defines/CommonDefines.hpp"

namespace ModelIdentification
{
	class ExternalFileDataParser : public boost::noncopyable
	{
		public :

			static bool parseSettings();
			static bool getSamples(std::queue<float> & dataQueue);
			static u64 getNumberOfLeftData();

		private :

			enum class EFileLineType : u8
			{
				EmptyLine,
				Comment,
				NumberOfSamples,
				SamplePeriod,
				Description,
				DataKeyWord,
				Data,
				Unknown
			};
			
			static void convertInputFileToUnixCoding();
			static EFileLineType determineLineType(const std::string & line);
			static bool getLineVariableValue(const std::string & line, std::string & value);
			static bool openFile();
			static bool closeFile();
			static bool validateFileExistence();
			static bool setFilePosForData();
			static const std::string & getLoggerPrefix();

			static std::mutex mMtx;
			static bool mAreSettingsParsed;
			static bool mAreFirstSamplesRead;
			static std::streampos mFilePos;
			static std::string mFilename;
			static std::ifstream mFileStream;
			static u64 mLeftDataCount;
			static const u64 mFileReadDataStep;
	};
}
