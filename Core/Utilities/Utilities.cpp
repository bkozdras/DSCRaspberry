#include "Utilities.hpp"

#include <algorithm>
#include <cctype>

namespace Utilities
{
    void conditionalExecutor(bool & condition, std::function<bool()> callback)
    {
        if (condition)
        {
            condition = callback();
        }
    }

    bool isDouble(const std::string & doubleString)
    {
        /*
        if (doubleString.empty())
        {
            return false;
        }

        auto isPointOccured = false;

        std::for_each
        (
            std::begin(doubleString),
            std::end(doubleString),
            [&isPointOccured](const char elem)
            {
                if (std::isdigit(elem))
                {
                }
                else if ( !isPointOccured && '.' == elem)
                {
                    isPointOccured = true;
                }
                else
                {
                    return false;
                }
            }
        );

        return true;
        */

        try
        {
            std::stod(doubleString);
            return true;
        }
        catch (std::invalid_argument exception)
        {
            return false;
        }
    }
}