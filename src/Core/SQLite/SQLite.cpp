#include "Core/SQLite/SQLite.hpp"

namespace SQLite
{
    SQLite::SQLite()
    {
        Storage::FileStore::mkdirRecursive(Storage::FileStore::BASE_PATH + "/db/");

        std::string path = Storage::FileStore::BASE_PATH + "/db/cookies.db";
        exit = sqlite3_open(path.c_str(), &db);

        if (exit)
        {
            std::cout << "Error open DB " << sqlite3_errmsg(db) << std::endl;
            return;
        }
    }

    SQLite::~SQLite()
    {
        sqlite3_close(db);
    }

    void SQLite::checkAndCreateTables()
    {
        char *messaggeError;
        exit = sqlite3_exec(db, checkQuery.c_str(), NULL, 0, &messaggeError);

        if (exit != SQLITE_OK)
        {
            std::cerr << "Error Checking Tables" << std::endl;
            sqlite3_free(messaggeError);
        }
        else
            std::cout << "Checking Finished Successfully" << std::endl;
    }

    void SQLite::insert(std::string table, std::string columns[], std::string values[])
    {
    }

    std::string SQLite::select(std::string table, std::string where)
    {
    }
} // namespace
