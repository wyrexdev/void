#pragma once

#include "System/User/User.hpp"

namespace System
{
    class Setup
    {
    public:
        bool isSetupNeeded();
        bool setup();
    };
} // namespace System
