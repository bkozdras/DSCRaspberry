#pragma once

#include <memory>

#include <boost/noncopyable.hpp>

#include "EInputDataSource.hpp"
#include "IInputDataGenerator.hpp"

namespace ModelIdentification
{
	class InputDataGeneratorCreator : public boost::noncopyable
	{
		public:
		
			static std::unique_ptr<IInputDataGenerator> create(EInputDataSource source);
	};
}
