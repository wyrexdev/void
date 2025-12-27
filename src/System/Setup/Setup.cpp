#include "System/Setup/Setup.hpp"

namespace System
{
    bool Setup::isSetupNeeded()
    {
        if (System::User::isExistUser("void") != 1)
            return true;

        return false;
    }

    bool Setup::setup() {
        if(isSetupNeeded()) {
            if(System::User::createUser()) {
                return true;
            } else {
                return false;
            }
        } else {
            return true;
        }

        return true;
    }
} // namespace System