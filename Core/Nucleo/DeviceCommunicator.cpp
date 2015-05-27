#include "DeviceCommunicator.hpp"
#include "MessageGateway.hpp"
#include "../RaspberryPeripherals/UartManager.hpp"
#include "../System/ThreadPool.hpp"
#include "../Utilities/Logger.hpp"
#include "../Utilities/ToStringConverter.hpp"

namespace Nucleo
{
    bool DeviceCommunicator::initialize()
    {
        UartManager::registerCallbackForNewMessage([](std::shared_ptr<TMessage> message){ newMessageCallback(message); });
        Logger::info("%s: Initialized!", getLoggerPrefix().c_str());
        return true;
    }

    #define SEND_MACRO_IMPL(messageUniqueName) void DeviceCommunicator::send(T##messageUniqueName##Request & request, std::function<void(T##messageUniqueName##Response &&, bool)> responseCallback)

    #define SEND_IMPL(messageUniqueName)    std::lock_guard<std::mutex> lockGuard(mMtx);                                                                                                                    \
                                            mCallbacksContainer[EMessageId_##messageUniqueName##Response].emplace_back                                                                                      \
                                            (                                                                                                                                                               \
                                                [responseCallback](std::shared_ptr<TMessage> message)                                                                                                       \
                                                {                                                                                                                                                           \
                                                    T##messageUniqueName##Response* response = (T##messageUniqueName##Response*)(message->data);                                                            \
                                                    responseCallback(std::move(*response), true);                                                                                                           \
                                                }                                                                                                                                                           \
                                            );                                                                                                                                                              \
                                            Logger::debug("%s: Message %s sent to Nucleo.", getLoggerPrefix().c_str(), ToStringConverter::getMessageId(EMessageId_##messageUniqueName##Request).c_str());   \
                                            MessageGateway::transmit(EMessageId_##messageUniqueName##Request, reinterpret_cast<TByte*>(&request))

    SEND_MACRO_IMPL(Polling)
    {
        SEND_IMPL(Polling);
    }

    SEND_MACRO_IMPL(ResetUnit)
    {
        SEND_IMPL(ResetUnit);
    }

    SEND_MACRO_IMPL(SetHeaterPower)
    {
        SEND_IMPL(SetHeaterPower);
    }

    SEND_MACRO_IMPL(SetHeaterTemperatureInFeedbackMode)
    {
        SEND_IMPL(SetHeaterTemperatureInFeedbackMode);
    }

    SEND_MACRO_IMPL(CallibreADS1248)
    {
        SEND_IMPL(CallibreADS1248);
    }

    SEND_MACRO_IMPL(SetChannelGainADS1248)
    {
        SEND_IMPL(SetChannelGainADS1248);
    }

    SEND_MACRO_IMPL(SetChannelSamplingSpeedADS1248)
    {
        SEND_IMPL(SetChannelSamplingSpeedADS1248);
    }

    SEND_MACRO_IMPL(StartRegisteringData)
    {
        SEND_IMPL(StartRegisteringData);
    }

    SEND_MACRO_IMPL(StopRegisteringData)
    {
        SEND_IMPL(StopRegisteringData);
    }

    SEND_MACRO_IMPL(SetNewDeviceModeADS1248)
    {
        SEND_IMPL(SetNewDeviceModeADS1248);
    }

    SEND_MACRO_IMPL(SetNewDeviceModeLMP90100ControlSystem)
    {
        SEND_IMPL(SetNewDeviceModeLMP90100ControlSystem);
    }

    SEND_MACRO_IMPL(SetNewDeviceModeLMP90100SignalsMeasurement)
    {
        SEND_IMPL(SetNewDeviceModeLMP90100SignalsMeasurement);
    }

    SEND_MACRO_IMPL(SetControlSystemType)
    {
        SEND_IMPL(SetControlSystemType);
    }

    SEND_MACRO_IMPL(SetControllerTunes)
    {
        SEND_IMPL(SetControllerTunes);
    }

    SEND_MACRO_IMPL(SetProcessModelParameters)
    {
        SEND_IMPL(SetProcessModelParameters);
    }

    SEND_MACRO_IMPL(SetControllingAlgorithmExecutionPeriod)
    {
        SEND_IMPL(SetControllingAlgorithmExecutionPeriod);
    }

    SEND_MACRO_IMPL(RegisterNewSegmentToProgram)
    {
        SEND_IMPL(RegisterNewSegmentToProgram);
    }

    SEND_MACRO_IMPL(DeregisterSegmentFromProgram)
    {
        SEND_IMPL(DeregisterSegmentFromProgram);
    }

    SEND_MACRO_IMPL(StartSegmentProgram)
    {
        SEND_IMPL(StartSegmentProgram);
    }

    SEND_MACRO_IMPL(StopSegmentProgram)
    {
        SEND_IMPL(StopSegmentProgram);
    }

    SEND_MACRO_IMPL(StartReferenceTemperatureStabilization)
    {
        SEND_IMPL(StartReferenceTemperatureStabilization);
    }

    SEND_MACRO_IMPL(StopReferenceTemperatureStabilization)
    {
        SEND_IMPL(StopReferenceTemperatureStabilization);
    }

    SEND_MACRO_IMPL(SetRTDPolynomialCoefficients)
    {
        SEND_IMPL(SetRTDPolynomialCoefficients);
    }

    #define INDICATION_CALLBACK_MACRO_IMPL(messageUniqueName) void DeviceCommunicator::registerIndCallback(std::function<void(T##messageUniqueName##Ind &&)> indCallback)

    #define INDICATION_IMPL(messageUniqueName)  std::lock_guard<std::mutex> lockGuard(mMtx);                                                            \
                                                mCallbacksContainer[EMessageId_##messageUniqueName##Ind].emplace_back                                   \
                                                (                                                                                                       \
                                                    [indCallback](std::shared_ptr<TMessage> message)                                                    \
                                                    {                                                                                                   \
                                                        T##messageUniqueName##Ind* indication = (T##messageUniqueName##Ind*)(message->data);            \
                                                        indCallback(std::move(*indication));                                                            \
                                                    }                                                                                                   \
                                                )

    INDICATION_CALLBACK_MACRO_IMPL(Log)
    {
        INDICATION_IMPL(Log);
    }

    INDICATION_CALLBACK_MACRO_IMPL(Fault)
    {
        INDICATION_IMPL(Fault);
    }

    INDICATION_CALLBACK_MACRO_IMPL(SampleCarrierData)
    {
        INDICATION_IMPL(SampleCarrierData);
    }

    INDICATION_CALLBACK_MACRO_IMPL(HeaterTemperature)
    {
        INDICATION_IMPL(HeaterTemperature);
    }

    INDICATION_CALLBACK_MACRO_IMPL(ReferenceTemperature)
    {
        INDICATION_IMPL(ReferenceTemperature);
    }

    INDICATION_CALLBACK_MACRO_IMPL(ControllerData)
    {
        INDICATION_IMPL(ControllerData);
    }

    INDICATION_CALLBACK_MACRO_IMPL(SegmentStarted)
    {
        INDICATION_IMPL(SegmentStarted);
    }

    INDICATION_CALLBACK_MACRO_IMPL(SegmentsProgramDone)
    {
        INDICATION_IMPL(SegmentsProgramDone);
    }

    INDICATION_CALLBACK_MACRO_IMPL(UnitReady)
    {
        INDICATION_IMPL(UnitReady);
    }

    INDICATION_CALLBACK_MACRO_IMPL(UnexpectedMasterMessage)
    {
        INDICATION_IMPL(UnexpectedMasterMessage);
    }

    void DeviceCommunicator::newMessageCallback(std::shared_ptr<TMessage> message)
    {
        std::lock_guard<std::mutex> lockGuard(mMtx);
        Logger::debug("%s: Received message %s from Nucleo.", getLoggerPrefix().c_str(), ToStringConverter::getMessageId(message->id).c_str());

        auto it = mCallbacksContainer.find(message->id);
        if (std::end(mCallbacksContainer) != it)
        {
            if (!isIndicationMessage(message->id))
            {
                ThreadPool::submit
                (
                    TaskPriority::Realtime,
                    [it, message]()
                    {
                        it->second.front()(message);
                    }
                );

                it->second.erase(std::begin(it->second));
            }
            else
            {
                for (auto & callback : it->second)
                {
                    ThreadPool::submit
                    (
                        TaskPriority::Realtime,
                        [callback, message]()
                        {
                            callback(message);
                        }
                    );
                }
            }
        }
    }

    bool DeviceCommunicator::isIndicationMessage(EMessageId messageId)
    {
        switch (messageId)
        {
            case EMessageId_ControllerDataInd :
            case EMessageId_FaultInd :
            case EMessageId_HeaterTemperatureInd :
            case EMessageId_LogInd :
            case EMessageId_ReferenceTemperatureInd :
            case EMessageId_SampleCarrierDataInd :
            case EMessageId_SegmentsProgramDoneInd :
            case EMessageId_SegmentStartedInd :
            case EMessageId_UnexpectedMasterMessageInd :
            case EMessageId_UnitReadyInd :
                return true;

            default :
                return false;
        }
    }

    const std::string & DeviceCommunicator::getLoggerPrefix()
    {
        static std::string loggerPrefix("DeviceCommunicator");
        return loggerPrefix;
    }

    std::map<EMessageId, std::vector<std::function<void(std::shared_ptr<TMessage>)>>> DeviceCommunicator::mCallbacksContainer;
    std::mutex DeviceCommunicator::mMtx;

    #undef SEND_MACRO_IMPL
    #undef SEND_IMPL
    #undef INDICATION_CALLBACK_MACRO_IMPL
    #undef INDICATION_IMPL
}