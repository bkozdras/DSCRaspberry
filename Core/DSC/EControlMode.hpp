#pragma once

#include "../Defines/CommonDefines.hpp"

enum class EControlMode : u8
{
    NotSet,
    OpenLoop,
    SimpleFeedback,
    MFCFeedback
};
