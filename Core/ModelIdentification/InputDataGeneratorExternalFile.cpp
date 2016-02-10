#include "InputDataGeneratorExternalFile.hpp"

#include "ExternalFileDataParser.hpp"
#include "../System/ThreadPool.hpp"

namespace ModelIdentification
{
	InputDataGeneratorExternalFile::InputDataGeneratorExternalFile()
	{

	}

	void InputDataGeneratorExternalFile::initialize()
	{
		std::lock_guard<std::mutex> lockGuard(mMtx);
		ExternalFileDataParser::getSamples(mDataQueue);
	}

	float InputDataGeneratorExternalFile::getNextInputValue()
	{
		std::lock_guard<std::mutex> lockGuard(mMtx);
		if (!mDataQueue.empty())
		{
			auto value = mDataQueue.front();
			mDataQueue.pop();
			if (20 > mDataQueue.size() && (0UL != ExternalFileDataParser::getNumberOfLeftData()))
			{
				ThreadPool::submit(TaskPriority::High, [this]() {
					ExternalFileDataParser::getSamples(mNewDataQueue);
					std::lock_guard<std::mutex> lockGuard(mMtx);
					while (!mNewDataQueue.empty())
					{
						// WEAK
						mDataQueue.push(mNewDataQueue.front());
						mNewDataQueue.pop();
					}
				});
			}
			return value;
		}
		return 0.0F;
	}

	void InputDataGeneratorExternalFile::validateInputData()
	{

	}

	InputDataGeneratorExternalFile::~InputDataGeneratorExternalFile()
	{

	}
}