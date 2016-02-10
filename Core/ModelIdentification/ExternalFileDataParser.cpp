#include "ExternalFileDataParser.hpp"

#include <regex>
#include <cstdlib>

#include <boost/filesystem.hpp>
#include <boost/tokenizer.hpp>

#include "../DSC/DataManager.hpp"
#include "../DSC/EDataType.hpp"
#include "../Utilities/Logger.hpp"
#include "../Utilities/Utilities.hpp"
#include "../SharedDefines/EUnitId.h"

namespace ModelIdentification
{
	bool ExternalFileDataParser::parseSettings()
	{
		std::lock_guard<std::mutex> lockGuard(mMtx);
		if (mAreSettingsParsed)
		{
			return true;
		}
		Logger::debug("%s: Parsing external file with input data to experiment...", getLoggerPrefix().c_str());
		if (!validateFileExistence())
		{
			Logger::error("%s: File does not exist. Parsing not done.", getLoggerPrefix().c_str());
			return false;
		}
		convertInputFileToUnixCoding();
		if (!openFile())
		{
			Logger::error("%s: Cannot open file. Parsing not done.", getLoggerPrefix().c_str());
			return false;
		}

		std::string fileLine;
		std::string value;
		EFileLineType lineType = EFileLineType::Unknown;

		mFileStream.seekg(0, mFileStream.beg);

		while (EFileLineType::DataKeyWord != lineType)
		{
			std::getline(mFileStream, fileLine);
			if (mFileStream.eof())
			{
				break;
			}
			lineType = determineLineType(fileLine);
			switch (lineType)
			{
				case EFileLineType::Comment:
				case EFileLineType::EmptyLine:
				case EFileLineType::DataKeyWord:
					break;

				case EFileLineType::Description:
				{
					if (!getLineVariableValue(fileLine, value))
					{
						Logger::error("%s: Description value validation failed.", getLoggerPrefix().c_str());
						Logger::error("%s: Value: %s.", getLoggerPrefix().c_str(), fileLine.c_str());
					}
					DSC::DataManager::updateUnitAttribute(EUnitId_Heater, "TestDataInputDescription", value);
					break;
				}

				case EFileLineType::NumberOfSamples:
				{
					if (!getLineVariableValue(fileLine, value) || !Utilities::isInteger(value))
					{
						Logger::error("%s: Number of Samples value validation failed.", getLoggerPrefix().c_str());
						Logger::error("%s: Value: %s.", getLoggerPrefix().c_str(), fileLine.c_str());
					}
					auto doubleValue = std::stod(value);
					DSC::DataManager::updateData(EDataType::ModelIdentificationExternalFileNumberOfSamples, doubleValue);
					mLeftDataCount = static_cast<u64>(doubleValue);
					break;
				}

				case EFileLineType::SamplePeriod:
				{
					if (!getLineVariableValue(fileLine, value) || !Utilities::isInteger(value))
					{
						Logger::error("%s: Sample Period value validation failed.", getLoggerPrefix().c_str());
						Logger::error("%s: Value: %s.", getLoggerPrefix().c_str(), fileLine.c_str());
					}
					DSC::DataManager::updateData(EDataType::ModelIdentificationExternalFileSamplingPeriod, std::stod(value));
					break;
				}

				default:
					break;
			}
		}
		closeFile();
		Logger::info("%s: External File with Input Data parsed with success!", getLoggerPrefix().c_str());

		mAreFirstSamplesRead = false;
		mAreSettingsParsed = true;

		return true;
	}

	bool ExternalFileDataParser::getSamples(std::queue<float> & dataQueue)
	{
		std::lock_guard<std::mutex> lockGuard(mMtx);
		mAreSettingsParsed = false;
		if (0UL == mLeftDataCount)
		{
			Logger::warning("%s: No data in external file to read...", getLoggerPrefix().c_str());
			return false;
		}

		Logger::debug("%s: Getting samples from file...", getLoggerPrefix().c_str());
		setFilePosForData();
		auto numberOfSamplesToRead = mFileReadDataStep;
		if (mLeftDataCount < mFileReadDataStep)
		{
			numberOfSamplesToRead = mLeftDataCount;
		}

		for (auto iter = 0; numberOfSamplesToRead > iter; ++iter)
		{
			if (!mFileStream.good())
			{
				closeFile();
				mLeftDataCount = 0U;
				Logger::error("%s: Reading data from file failed.", getLoggerPrefix().c_str());
				return false;
			}
			float valueToRead;
			mFileStream >> valueToRead;
			dataQueue.push(valueToRead);
		}
		mFilePos = mFileStream.tellg();
		mLeftDataCount = mLeftDataCount - numberOfSamplesToRead;
		closeFile();
		return true;
	}

	u64 ExternalFileDataParser::getNumberOfLeftData()
	{
		std::lock_guard<std::mutex> lockGuard(mMtx);
		return mLeftDataCount;
	}

	void ExternalFileDataParser::convertInputFileToUnixCoding()
	{
		std::string command("dos2unix " + mFilename);
		system(command.c_str());
		Logger::info("%s: File with input data converted from DOS to UNIX coding format.", getLoggerPrefix().c_str());
	}

	ExternalFileDataParser::EFileLineType ExternalFileDataParser::determineLineType(const std::string & line)
	{
		if (line.empty())
		{
			return EFileLineType::EmptyLine;
		}

		if ('#' == line.front())
		{
			return EFileLineType::Comment;
		}

		{
			std::regex regExp("numberOfSamples.*");
			if (std::regex_match(line, regExp))
			{
				return EFileLineType::NumberOfSamples;
			}
		}

		{
			std::regex regExp("samplePeriod.*");
			if (std::regex_match(line, regExp))
			{
				return EFileLineType::SamplePeriod;
			}
		}

		{
			std::regex regExp("description.*");
			if (std::regex_match(line, regExp))
			{
				return EFileLineType::Description;
			}
		}
		
		{
			std::regex regExp("data.*");
			if (std::regex_match(line, regExp))
			{
				return EFileLineType::DataKeyWord;
			}
		}

		if (Utilities::isDouble(line))
		{
			return EFileLineType::Data;
		}

		return EFileLineType::Unknown;
	}

	bool ExternalFileDataParser::getLineVariableValue(const std::string & line, std::string & value)
	{
		boost::char_separator<char> separator("=");
		boost::tokenizer<boost::char_separator<char>> tokens(line, separator);
		std::vector<std::string> splitLine(std::begin(tokens), std::end(tokens));
		if (2 != splitLine.size())
		{
			return false;
		}
		value = splitLine.back();
		return true;
	}

	bool ExternalFileDataParser::openFile()
	{
		mFileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try
		{
			mFileStream.open(mFilename);
			return true;
		}
		catch (std::ifstream::failure err)
		{
			Logger::error("%s: Opening file: %s failure.", getLoggerPrefix().c_str(), mFilename.c_str());
			return false;
		}
	}

	bool ExternalFileDataParser::closeFile()
	{
		try
		{
			mFileStream.close();
			return true;
		}
		catch (std::ifstream::failure err)
		{
			Logger::error("%s: Closing file: %s failure.", getLoggerPrefix().c_str(), mFilename.c_str());
			return false;
		}
	}

	bool ExternalFileDataParser::validateFileExistence()
	{
		std::string filename = "";
		auto isFilenameLoaded = DSC::DataManager::getUnitAttribute(EUnitId_Heater, "TestDataInputFilename", filename);

		if (!isFilenameLoaded)
		{
			Logger::error("%s: No filename loaded...", getLoggerPrefix().c_str());
			return false;
		}

		boost::filesystem::path filePath = filename;

		if (!boost::filesystem::exists(filePath))
		{
			Logger::error("%s: File %s does not exist.", getLoggerPrefix().c_str(), filename.c_str());
			return false;
		}

		Logger::debug("%s: File %s exists.", getLoggerPrefix().c_str(), filename.c_str());
		mFilename = filename;

		return true;
	}

	bool ExternalFileDataParser::setFilePosForData()
	{
		if (!openFile())
		{
			return false;
		}
		if (!mAreFirstSamplesRead)
		{
			std::string line;
			EFileLineType type = EFileLineType::Unknown;
			mFileStream.seekg(0, mFileStream.beg);
			while (mFileStream.good() && (EFileLineType::DataKeyWord != type))
			{
				std::getline(mFileStream, line);
				type = determineLineType(line);
			}
			if (EFileLineType::DataKeyWord == type)
			{
				mFilePos = mFileStream.tellg();
				mAreFirstSamplesRead = true;
				return true;
			}
			else
			{
				Logger::error("%s: Setting file position at the data content begin failed...", getLoggerPrefix().c_str());
				return false;
			}
		}
		else
		{
			mFileStream.seekg(mFilePos);
			return true;
		}
	}

	const std::string & ExternalFileDataParser::getLoggerPrefix()
	{
		static const std::string prefix("ExternalFileDataParser");
		return prefix;
	}

	std::mutex ExternalFileDataParser::mMtx;
	bool ExternalFileDataParser::mAreSettingsParsed = false;
	bool ExternalFileDataParser::mAreFirstSamplesRead = false;
	std::streampos ExternalFileDataParser::mFilePos;
	std::string ExternalFileDataParser::mFilename;
	std::ifstream ExternalFileDataParser::mFileStream;
	u64 ExternalFileDataParser::mLeftDataCount;
	const u64 ExternalFileDataParser::mFileReadDataStep = 1UL;
}
