#pragma once

#include <string>
#include "../SharedDefines/EFaultId.h"
#include "../SharedDefines/EUnitId.h"
#include "../SharedDefines/EMessageId.h"
#include "../SharedDefines/ADS1248Types.h"
#include "../SharedDefines/LMP90100Types.h"

namespace ToStringConverter
{
    const std::string & getFaultId(EFaultId faultId);
    const std::string & getUnitId(EUnitId unitId);
    const std::string & getMessageId(EMessageId messageId);
    const std::string & getADS1248Mode(EADS1248Mode mode);
    const std::string & getADS1248CallibrationType(EADS1248CallibrationType type);
    const std::string & getADS1248GainValue(EADS1248GainValue value);
    const std::string & getADS1248SamplingSpeed(EADS1248SamplingSpeed speed);
    const std::string & getLMP90100Mode(ELMP90100Mode mode);
}