#include "System/User/User.hpp"

namespace System
{
    bool User::isExistUser(std::string user) {
        return getpwnam(user.c_str()) != nullptr;
    }

    bool User::createUser() {
        
    }    
} // namespace System
