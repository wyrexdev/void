#include "System/User/User.hpp"

namespace System
{
    bool User::isExistUser(std::string user)
    {
        return getpwnam(user.c_str()) != nullptr;
    }

    bool User::createUser()
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            execlp(
                "pkexec",
                "pkexec",
                "/usr/sbin/useradd",
                "-r",
                "-s",
                "/usr/sbin/nologin",
                "void",
                nullptr);
            _exit(1);
        }

        int status = 0;
        waitpid(pid, &status, 0);

        return WIFEXITED(status) && WEXITSTATUS(status) == 0;
    }

} // namespace System
