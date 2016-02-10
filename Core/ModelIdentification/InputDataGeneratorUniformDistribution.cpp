#include "InputDataGeneratorUniformDistribution.hpp"

namespace ModelIdentification
{
	InputDataGeneratorUniformDistribution::InputDataGeneratorUniformDistribution()
	{

	}

	void InputDataGeneratorUniformDistribution::initialize()
	{
		mRandomEngine = std::make_unique<std::default_random_engine>();
		mUniformDistributionGenerator = std::make_unique<std::uniform_real_distribution<float>>(mRangeMin, mRangeMax);
		(*mUniformDistributionGenerator)(*mRandomEngine);
	}

	float InputDataGeneratorUniformDistribution::getNextInputValue()
	{
		return (*mUniformDistributionGenerator)(*mRandomEngine);
	}

	void InputDataGeneratorUniformDistribution::validateInputData()
	{

	}

	InputDataGeneratorUniformDistribution::~InputDataGeneratorUniformDistribution()
	{
	}
}