#include "System/User/User.hpp"

namespace System
{
    bool User::isExistUser(std::string user)
    {
        struct passwd pwd;
        struct passwd *result = nullptr;

        std::vector<char> buffer(16384);

        int ret = getpwnam_r(
            user.c_str(),
            &pwd,
            buffer.data(),
            buffer.size(),
            &result);

        return (ret == 0 && result != nullptr);
    }

    bool User::createUser()
    {
        int ret = system("pkexec sudo useradd -r -s /usr/sbin/nologin void");
        if (ret != 0)
        {
            return false;
        }

        return true;
    }
} // namespace System
