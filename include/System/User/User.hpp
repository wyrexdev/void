#pragma once

#include <string>

#include <pwd.h>

namespace System
{
    class User
    {
    public:
        static bool isExistUser(std::string user);
        static bool createUser();
    };
} // namespace User