#pragma once

#include <functional>

namespace Utilities
{
    void conditionalExecutor(bool & condition, std::function<bool()> callback);
    bool isDouble(const std::string & doubleString);
    bool isInteger(const std::string & integerString);
}