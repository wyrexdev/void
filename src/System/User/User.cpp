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
                "/bin/sh",
                "-c",
                "useradd -r -s /usr/sbin/nologin " + username + "|| exit 1; "
                "mkdir -p /home/" + username + " && "
                "chown " + username + ":" + username + " /home/" + username + " && "
                "chmod 700 /home/" + username,
                nullptr);
            _exit(1);
        }

        int status = 0;
        waitpid(pid, &status, 0);

        return WIFEXITED(status) && WEXITSTATUS(status) == 0;
    }

    std::string User::getCurrentUser()
    {
        struct passwd *pw = getpwuid(geteuid());
        return pw->pw_name;
    }

    User::UserIds User::getUserIds()
    {
        struct passwd *pw = getpwnam(username.c_str());
        if (!pw)
        {
            throw std::runtime_error(username + " user not found");
        }

        return {
            .uid = pw->pw_uid,
            .gid = pw->pw_gid};
    }

} // namespace System
