#include "Utilities.hpp"

namespace Utilities
{
    void conditionalExecutor(bool & condition, std::function<bool()> callback)
    {
        if (condition)
        {
            condition = callback();
        }
    }
}