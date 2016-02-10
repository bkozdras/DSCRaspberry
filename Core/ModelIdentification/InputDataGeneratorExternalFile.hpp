#pragma once

#include "IInputDataGenerator.hpp"

#include <mutex>
#include <queue>

namespace ModelIdentification
{
	class InputDataGeneratorExternalFile : public IInputDataGenerator
	{
		public:

			InputDataGeneratorExternalFile();

			void initialize() override;
			float getNextInputValue() override;
			void validateInputData() override;

			~InputDataGeneratorExternalFile();

		private :

			std::mutex mMtx;
			std::queue<float> mDataQueue;
			std::queue<float> mNewDataQueue;
	};
}
