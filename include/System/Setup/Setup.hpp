#pragma once

#include "System/User/User.hpp"

#include "Utils/File/FS.hpp"

namespace System
{
    class Setup
    {
    public:
        bool isSetupNeeded();
        bool setup();
    };
} // namespace System
