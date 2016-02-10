#pragma once

namespace ModelIdentification
{
	class IInputDataGenerator
	{
		public :

			virtual void initialize() = 0;
			virtual float getNextInputValue() = 0;
			virtual void validateInputData() = 0;

			virtual ~IInputDataGenerator() {};
	};
}
