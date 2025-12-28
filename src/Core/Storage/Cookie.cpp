#include "Core/Storage/Cookie.hpp"

namespace Storage
{
    Cookie::Cookie()
    {
        FileStore::mkdirRecursive(FileStore::BASE_PATH + "/db/");

        std::string path = FileStore::BASE_PATH + "/db/cookies.db";
        exit = sqlite3_open(path.c_str(), &db);

        if (exit)
        {
            std::cout << "Error open DB " << sqlite3_errmsg(db) << std::endl;
            return;
        }
    }
}