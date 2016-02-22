#pragma once

#include "IInputDataGenerator.hpp"

#include <memory>
#include <random>

namespace ModelIdentification
{
	class InputDataGeneratorUniformDistribution : public IInputDataGenerator
	{
		public:

			InputDataGeneratorUniformDistribution();

			void initialize() override;
			float getNextInputValue() override;
			void validateInputData() override;

			~InputDataGeneratorUniformDistribution();

		private :

			const float mRangeMin = 0.0F;
			const float mRangeMax = 60.0F;
			std::unique_ptr<std::default_random_engine> mRandomEngine = std::unique_ptr<std::default_random_engine>();
			std::unique_ptr<std::uniform_int_distribution<int>> mUniformDistributionGenerator = std::unique_ptr<std::uniform_int_distribution<int>>();
	};
}
