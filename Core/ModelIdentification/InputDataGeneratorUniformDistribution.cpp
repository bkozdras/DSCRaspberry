#include "InputDataGeneratorUniformDistribution.hpp"

namespace ModelIdentification
{
	InputDataGeneratorUniformDistribution::InputDataGeneratorUniformDistribution()
	{

	}

	void InputDataGeneratorUniformDistribution::initialize()
	{
		mRandomEngine = std::make_unique<std::default_random_engine>();
		mUniformDistributionGenerator = std::make_unique<std::uniform_int_distribution<int>>(static_cast<int>(mRangeMin * 10.0F), static_cast<int>(mRangeMax * 10.0F));
		(*mUniformDistributionGenerator)(*mRandomEngine);
	}

	float InputDataGeneratorUniformDistribution::getNextInputValue()
	{
		return static_cast<float>((*mUniformDistributionGenerator)(*mRandomEngine)) / 10.0F;
	}

	void InputDataGeneratorUniformDistribution::validateInputData()
	{

	}

	InputDataGeneratorUniformDistribution::~InputDataGeneratorUniformDistribution()
	{
	}
}