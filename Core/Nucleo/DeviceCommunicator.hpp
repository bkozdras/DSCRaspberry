#pragma once

#include <boost/noncopyable.hpp>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <vector>
#include "../SharedDefines/EMessageId.h"
#include "../SharedDefines/MessagesDefines.h"
#include "../SharedDefines/TMessage.h"

namespace Nucleo
{
    class DeviceCommunicator : public boost::noncopyable
    {
        public :

            static bool initialize();

            #define SEND_MACRO(messageUniqueName) static void send(T##messageUniqueName##Request & request, std::function<void(T##messageUniqueName##Response &&, bool)> responseCallback = std::function<void(T##messageUniqueName##Response &&, bool)>())

            SEND_MACRO(Polling);
            SEND_MACRO(ResetUnit);
            SEND_MACRO(SetHeaterPower);
            SEND_MACRO(SetHeaterTemperatureInFeedbackMode);
            SEND_MACRO(CallibreADS1248);
            SEND_MACRO(SetChannelGainADS1248);
            SEND_MACRO(SetChannelSamplingSpeedADS1248);
            SEND_MACRO(StartRegisteringData);
            SEND_MACRO(StopRegisteringData);
            SEND_MACRO(SetNewDeviceModeADS1248);
            SEND_MACRO(SetNewDeviceModeLMP90100ControlSystem);
            SEND_MACRO(SetNewDeviceModeLMP90100SignalsMeasurement);
            SEND_MACRO(SetControlSystemType);
            SEND_MACRO(SetControllerTunes);
            SEND_MACRO(SetProcessModelParameters);
            SEND_MACRO(SetControllingAlgorithmExecutionPeriod);
            SEND_MACRO(RegisterNewSegmentToProgram);
            SEND_MACRO(DeregisterSegmentFromProgram);
            SEND_MACRO(StartSegmentProgram);
            SEND_MACRO(StopSegmentProgram);
            SEND_MACRO(StartReferenceTemperatureStabilization);
            SEND_MACRO(StopReferenceTemperatureStabilization);
            SEND_MACRO(SetRTDPolynomialCoefficients);

            #define INDICATION_CALLBACK_MACRO(messageUniqueName) static void registerIndCallback(std::function<void(T##messageUniqueName##Ind &&)> indCallback)

            INDICATION_CALLBACK_MACRO(Log);
            INDICATION_CALLBACK_MACRO(Fault);
            INDICATION_CALLBACK_MACRO(SampleCarrierData);
            INDICATION_CALLBACK_MACRO(HeaterTemperature);
            INDICATION_CALLBACK_MACRO(ReferenceTemperature);
            INDICATION_CALLBACK_MACRO(ControllerData);
            INDICATION_CALLBACK_MACRO(SegmentStarted);
            INDICATION_CALLBACK_MACRO(SegmentsProgramDone);
            INDICATION_CALLBACK_MACRO(UnitReady);
            INDICATION_CALLBACK_MACRO(UnexpectedMasterMessage);

            #undef SEND_MACRO
            #undef INDICATION_CALLBACK_MACRO

        private :

            static std::map<EMessageId, std::vector<std::function<void(std::shared_ptr<TMessage>)>>> mCallbacksContainer;
            static std::mutex mMtx;

            static void newMessageCallback(std::shared_ptr<TMessage> message);
            static bool isIndicationMessage(EMessageId messageId);
            static void deserialize(void );
            static const std::string & getLoggerPrefix();
    };
}