#include "ToStringConverter.hpp"
#include <map>

namespace ToStringConverter
{
    const std::string & getFaultId(EFaultId faultId)
    {
        static std::map<EFaultId, std::string> faultIdToString = decltype(faultIdToString)
        {
            std::make_pair(EFaultId_Assert, "Assert"),
            std::make_pair(EFaultId_Communication, "Communication"),
            std::make_pair(EFaultId_CRCFailure, "CRCFailure"),
            std::make_pair(EFaultId_I2C, "I2C"),
            std::make_pair(EFaultId_NoMemory, "NoMemory"),
            std::make_pair(EFaultId_OverCurrent, "OverCurrent"),
            std::make_pair(EFaultId_Spi, "Spi"),
            std::make_pair(EFaultId_System, "System"),
            std::make_pair(EFaultId_Uart, "Uart"),
            std::make_pair(EFaultId_WrongData, "WrongData"),
            std::make_pair(EFaultId_Unknown, "Unknown")
        };

        return faultIdToString[faultId];
    }

    const std::string & getUnitId(EUnitId unitId)
    {
        static std::map<EUnitId, std::string> unitIdToString = decltype(unitIdToString)
        {
            std::make_pair(EUnitId_ADS1248, "ADS1248"),
            std::make_pair(EUnitId_DRV595, "DRV595"),
            std::make_pair(EUnitId_Empty, "Empty"),
            std::make_pair(EUnitId_LMP90100ControlSystem, "LMP90100 CS"),
            std::make_pair(EUnitId_LMP90100SignalsMeasurement, "LMP90100 SM"),
            std::make_pair(EUnitId_MCP4716, "MCP4716"),
            std::make_pair(EUnitId_Nucleo, "Nucleo"),
            std::make_pair(EUnitId_Peltier, "Peltier"),
            std::make_pair(EUnitId_Raspberry, "Raspberry"),
            std::make_pair(EUnitId_Rtd1Pt100, "RTD1 Pt100"),
            std::make_pair(EUnitId_Rtd2Pt100, "RTD2 Pt100"),
            std::make_pair(EUnitId_RtdPt1000, "RTD Pt1000"),
            std::make_pair(EUnitId_Thermocouple1, "Thermocouple1"),
            std::make_pair(EUnitId_Thermocouple2, "Thermocouple2"),
            std::make_pair(EUnitId_Thermocouple3, "Thermocouple3"),
            std::make_pair(EUnitId_Thermocouple4, "Thermocouple4"),
            std::make_pair(EUnitId_ThermocoupleReference, "Thermocouple Reference"),
            std::make_pair(EUnitId_Unknown, "Unknown")
        };

        return unitIdToString[unitId];
    }
}
