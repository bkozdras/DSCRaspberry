#include "InputDataGeneratorCreator.hpp"

#include "InputDataGeneratorExternalFile.hpp"
#include "InputDataGeneratorUniformDistribution.hpp"

namespace ModelIdentification
{
	std::unique_ptr<IInputDataGenerator> InputDataGeneratorCreator::create(EInputDataSource source)
	{
		if (EInputDataSource::ExternalFile == source)
		{
			return std::make_unique<InputDataGeneratorExternalFile>();
		}
		else
		{
			return std::make_unique<InputDataGeneratorUniformDistribution>();
		}
	}
}
