#include "ExperimentManager.hpp"

#include "../Utilities/Logger.hpp"

#include "../DSC/DataManager.hpp"
#include "../DSC/HeaterManager.hpp"
#include "../DSC/EDataType.hpp"
#include "../SharedDefines/EUnitId.h"
#include "../System/ThreadPool.hpp"

#include "InputDataGeneratorCreator.hpp"

namespace ModelIdentification
{
	bool ExperimentManager::initialize()
	{
		std::lock_guard<std::mutex> lockGuard(mMtx);

		Logger::debug("%s: Initializing...", getLoggerPrefix().c_str());

		DSC::DataManager::updateData(EDataType::ModelIdentificationExperimentTime, DSC::DataManager::UnknownValue);
		DSC::DataManager::updateData(EDataType::ModelIdentificationNumberOfSamples, DSC::DataManager::UnknownValue);
		DSC::DataManager::updateData(EDataType::ModelIdentificationSamplingPeriod, DSC::DataManager::UnknownValue);
		DSC::DataManager::updateData(EDataType::ModelIdentificationExperimentalInfoExpiredSamples, DSC::DataManager::UnknownValue);
		DSC::DataManager::updateData(EDataType::ModelIdentificationExperimentalInfoLeftSamples, DSC::DataManager::UnknownValue);
		DSC::DataManager::updateData(EDataType::ModelIdentificationExperimentalInfoExpiredTime, DSC::DataManager::UnknownValue);
		DSC::DataManager::updateData(EDataType::ModelIdentificationExperimentalInfoLeftTime, DSC::DataManager::UnknownValue);
		DSC::DataManager::updateUnitAttribute(EUnitId_Heater, "TestDataInputFilename", "N/A");
		DSC::DataManager::updateUnitAttribute(EUnitId_Heater, "TestDataInputDescription", "N/A");
		DSC::DataManager::updateUnitAttribute(EUnitId_Heater, "TestInputExperimentInfoState", "Idle");

		Logger::info("%s: Initialized!", getLoggerPrefix().c_str());

		return true;
	}

	bool ExperimentManager::startExperiment()
	{
		std::lock_guard<std::mutex> lockGuard(mMtx);

		if (EExperimentState::Running == mExperimentState)
		{
			Logger::warning("%s: Experiment is running. Cannot to start again.", getLoggerPrefix().c_str());
			return false;
		}

		mInputDataGenerator->initialize();
		initializeExperimentValues();

		const auto samplingPeriod = static_cast<u64>(DSC::DataManager::getData(EDataType::ModelIdentificationSamplingPeriod));
		mExperimentTimerId = TimerManager::create(samplingPeriod, samplingPeriod, []() {
			std::lock_guard<std::mutex> lockGuard(mMtx);
			nextExperimentValueCallback();
		});

		Logger::info("%s: Experiment started. CV and PV values are being collected.", getLoggerPrefix().c_str());

		return true;
	}
	
	bool ExperimentManager::forceStoppingExperiment()
	{
		std::lock_guard<std::mutex> lockGuard(mMtx);

		if (EExperimentState::Idle == mExperimentState)
		{
			Logger::warning("%s: Experiment is not running. Cannot to stop.", getLoggerPrefix().c_str());
			return false;
		}

		stopExperiment();

		Logger::info("%s: Experiment stopped (by user).", getLoggerPrefix().c_str());

		return true;
	}

	void ExperimentManager::validateInputData()
	{
		std::lock_guard<std::mutex> lockGuard(mMtx);
		if (mInputDataGenerator)
		{
			mInputDataGenerator->validateInputData();
		}
	}

	bool ExperimentManager::setInputDataSource(EInputDataSource inputDataSource)
	{
		std::lock_guard<std::mutex> lockGuard(mMtx);
		if (EExperimentState::Idle == mExperimentState)
		{
			mInputDataGenerator = InputDataGeneratorCreator::create(inputDataSource);
			if (EInputDataSource::ExternalFile == inputDataSource)
			{
				Logger::info("%s: Set input data source for: External File!", getLoggerPrefix().c_str());
				std::string filename = "";
				DSC::DataManager::getUnitAttribute(EUnitId_Heater, "TestDataInputFilename", filename);
				Logger::info("%s: File with inputData: %s.", getLoggerPrefix().c_str(), filename.c_str());
			}
			else
			{
				Logger::info("%s: Set input data source for: Uniform Distribution!", getLoggerPrefix().c_str());
			}
			mInputDataSource = inputDataSource;
			return true;
		}
		else
		{
			Logger::warning("%s: Experiment is running. Cannot change input data source!", getLoggerPrefix().c_str());
			return false;
		}
	}

	EInputDataSource ExperimentManager::getInputDataSource()
	{
		std::lock_guard<std::mutex> lockGuard(mMtx);
		return mInputDataSource;
	}

	EExperimentState ExperimentManager::getExperimentState()
	{
		std::lock_guard<std::mutex> lockGuard(mMtx);
		return mExperimentState;
	}

	void ExperimentManager::stopExperiment()
	{
		TimerManager::destroy(mExperimentTimerId);
		ThreadPool::submit(TaskPriority::Realtime, []() {
			DSC::HeaterManager::setPowerInPercent(0.0F);
		});
		deinitializeExperimentValues();
	}

	void ExperimentManager::nextExperimentValueCallback()
	{
		const auto leftSamples = DSC::DataManager::getData(EDataType::ModelIdentificationExperimentalInfoLeftSamples);
		if (0U == leftSamples)
		{
			stopExperiment();
			Logger::info("%s: Experiment stopped. All required samples are collected.", getLoggerPrefix().c_str());
			return;
		}
		const auto nextValue = mInputDataGenerator->getNextInputValue();
		ThreadPool::submit(TaskPriority::Realtime, [nextValue]() {
			DSC::HeaterManager::setPowerInPercent(nextValue);
		});
		ThreadPool::submit(TaskPriority::Normal, [leftSamples]() {
			const auto expiredSamples = DSC::DataManager::getData(EDataType::ModelIdentificationExperimentalInfoExpiredSamples);
			const auto expiredTime = DSC::DataManager::getData(EDataType::ModelIdentificationExperimentalInfoExpiredTime);
			const auto leftTime = DSC::DataManager::getData(EDataType::ModelIdentificationExperimentalInfoLeftTime);
			DSC::DataManager::updateData(EDataType::ModelIdentificationExperimentalInfoExpiredSamples, expiredSamples + 1.0);
			DSC::DataManager::updateData(EDataType::ModelIdentificationExperimentalInfoLeftSamples, leftSamples - 1.0);
			DSC::DataManager::updateData(EDataType::ModelIdentificationExperimentalInfoExpiredTime, expiredTime + mSamplingPeriod);
			DSC::DataManager::updateData(EDataType::ModelIdentificationExperimentalInfoLeftTime, leftTime - mSamplingPeriod);
		});
	}

	void ExperimentManager::initializeExperimentValues()
	{
		const auto samplePeriod = DSC::DataManager::getData(EDataType::ModelIdentificationSamplingPeriod);
		const auto numberOfSamples = DSC::DataManager::getData(EDataType::ModelIdentificationNumberOfSamples);
		const auto experimentTime = DSC::DataManager::getData(EDataType::ModelIdentificationExperimentTime);
		DSC::DataManager::updateData(EDataType::ModelIdentificationExperimentalInfoExpiredSamples, 0.0);
		DSC::DataManager::updateData(EDataType::ModelIdentificationExperimentalInfoLeftSamples, numberOfSamples);
		DSC::DataManager::updateData(EDataType::ModelIdentificationExperimentalInfoExpiredTime, 0.0);
		DSC::DataManager::updateData(EDataType::ModelIdentificationExperimentalInfoLeftTime, experimentTime);
		DSC::DataManager::updateUnitAttribute(EUnitId_Heater, "TestInputExperimentInfoState", "Running");
		mExperimentState = EExperimentState::Running;
		mSamplingPeriod = samplePeriod;
	}

	void ExperimentManager::deinitializeExperimentValues()
	{
		DSC::DataManager::updateData(EDataType::ModelIdentificationExperimentalInfoExpiredSamples, DSC::DataManager::UnknownValue);
		DSC::DataManager::updateData(EDataType::ModelIdentificationExperimentalInfoLeftSamples, DSC::DataManager::UnknownValue);
		DSC::DataManager::updateData(EDataType::ModelIdentificationExperimentalInfoExpiredTime, DSC::DataManager::UnknownValue);
		DSC::DataManager::updateData(EDataType::ModelIdentificationExperimentalInfoLeftTime, DSC::DataManager::UnknownValue);
		DSC::DataManager::updateUnitAttribute(EUnitId_Heater, "TestInputExperimentInfoState", "Idle");
		mExperimentState = EExperimentState::Idle;
	}

	const std::string & ExperimentManager::getLoggerPrefix()
	{
		static const std::string prefix("ExperimentManager");
		return prefix;
	}

	std::mutex ExperimentManager::mMtx;
	TimerManager::TimerId ExperimentManager::mExperimentTimerId;
	std::unique_ptr<IInputDataGenerator> ExperimentManager::mInputDataGenerator;
	EExperimentState ExperimentManager::mExperimentState = EExperimentState::Idle;
	EInputDataSource ExperimentManager::mInputDataSource = EInputDataSource::Unknown;
	double ExperimentManager::mSamplingPeriod = 0.0;
}
