#pragma once

#include <boost/noncopyable.hpp>

class UnitsDetector : boost::noncopyable
{
    public :

        enum class Status
        {
            Detected,
            Lost,
            Unknown
        };
};