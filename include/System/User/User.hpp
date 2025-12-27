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
        struct UserIds
        {
            uid_t uid;
            gid_t gid;
        };

        static bool isExistUser(std::string user);
        static bool createUser();

        static std::string getCurrentUser();

        static UserIds getUserIds();
        
        inline static std::string username = "void";
    };
} // namespace User