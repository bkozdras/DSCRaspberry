#pragma once

#include <string>
#include "../SharedDefines/EFaultId.h"
#include "../SharedDefines/EUnitId.h"
#include "../SharedDefines/EMessageId.h"

namespace ToStringConverter
{
    const std::string & getFaultId(EFaultId faultId);
    const std::string & getUnitId(EUnitId unitId);
    const std::string & getMessageId(EMessageId messageId);
}