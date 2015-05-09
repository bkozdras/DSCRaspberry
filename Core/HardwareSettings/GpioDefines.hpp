#pragma once

#include <boost/noncopyable.hpp>
#include <map>
#include <string>

class RaspberryGpio : public boost::noncopyable
{
    public :

        enum class Type
        {
            PeripheralsPowerOn,
            LedRaspberry,
            LedNucleo,
            LedReferenceTemperatureController,
            LedSignalConditionerForControlSystem,
            LedSignalsMeasurementBoard,
            LedReserved
        };

        static const std::string & getNumber(Type gpioType)
        {
            static std::map<Type, std::string> typeToString = decltype(typeToString)
            {
                std::make_pair(Type::PeripheralsPowerOn, "23"),
                std::make_pair(Type::LedRaspberry, "22"),
                std::make_pair(Type::LedNucleo, "27"),
                std::make_pair(Type::LedReferenceTemperatureController, "17"),
                std::make_pair(Type::LedSignalConditionerForControlSystem, "4"),
                std::make_pair(Type::LedSignalsMeasurementBoard, "3"),
                std::make_pair(Type::LedReserved, "2")
            };

            return typeToString[gpioType];
        }
};