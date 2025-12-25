#include "System/User/User.hpp"

namespace System
{
    bool User::isExistUser(std::string user)
    {
        return getpwnam(user.c_str()) != nullptr;
    }

    bool User::createUser(std::string user)
    {
        int ret = system("useradd -r -s /usr/sbin/nologin void");
        if (ret != 0)
        {
            return false;
        }

        return true;
    }
} // namespace System
