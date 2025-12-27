#pragma once

#include <unistd.h>
#include <sys/wait.h>
#include <pwd.h>
#include <cstdlib>

#include <iostream>

#include <vector>
#include <string>

namespace System
{
    class User
    {
    public:
        static bool isExistUser(std::string user);
        static bool createUser();
    };
} // namespace User