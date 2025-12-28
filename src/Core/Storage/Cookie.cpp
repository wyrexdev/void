#include "Core/Storage/Cookie.hpp"

namespace IPC
{
    Cookie::Cookie()
    {
        std::string path = std::string(SANDBOX_ROOT) + "/Cookies.db";
        exit = sqlite3_open(path.c_str(), &db);

        if (exit)
        {
            std::cout << "Error open DB " << sqlite3_errmsg(db) << std::endl;
            return;
        }
    }
}