#pragma once

#include <memory>
#include <mutex>
#include <string>

#include <boost/noncopyable.hpp>

#include "EInputDataSource.hpp"
#include "EExperimentState.hpp"
#include "IInputDataGenerator.hpp"
#include "../System/TimerManager.hpp"

namespace ModelIdentification
{
	class ExperimentManager : public boost::noncopyable
	{
		public:

			static bool initialize();

			static bool startExperiment();
			static bool forceStoppingExperiment();
			static void validateInputData();

			static bool setInputDataSource(EInputDataSource inputDataSource);
			static EInputDataSource getInputDataSource();
			static EExperimentState getExperimentState();

		private:

			static void stopExperiment();
			static void nextExperimentValueCallback();
			static void initializeExperimentValues();
			static void deinitializeExperimentValues();

			static const std::string & getLoggerPrefix();

			static std::mutex mMtx;
			static TimerManager::TimerId mExperimentTimerId;
			static std::unique_ptr<IInputDataGenerator> mInputDataGenerator;
			static EExperimentState mExperimentState;
			static EInputDataSource mInputDataSource;
			static double mSamplingPeriod;
	};
}
