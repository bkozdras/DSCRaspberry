#pragma once

#include <boost/noncopyable.hpp>

namespace Nucleo
{
    class UnitsDetector : boost::noncopyable
    {
        public:

            enum class Status
            {
                Detected,
                Lost,
                Unknown
            };
    };
}