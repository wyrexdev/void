#pragma once

#include <string>

#include <cstdlib>
#include <pwd.h>

namespace System
{
    class User
    {
    public:
        static bool isExistUser(std::string user);
        static bool createUser(std::string user);
    };
} // namespace User