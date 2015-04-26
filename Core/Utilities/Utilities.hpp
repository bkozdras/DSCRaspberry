#pragma once

#include <functional>

namespace Utilities
{
    void conditionalExecutor(bool & condition, std::function<bool()> callback);
}