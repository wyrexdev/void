#pragma once

#include <string>
#include <sqlite3.h>

#include "Core/Storage/FileStore.hpp"
#include "Utils/File/FS.hpp"

namespace SQLite
{
    class SQLite
    {
    public:
        SQLite();
        ~SQLite();

        void checkAndCreateTables();

        void insert(std::string table, std::string columns[], std::string values[]);
        std::string select(std::string table, std::string where);

    private:
        int exit = 0;
        sqlite3 *db;

        std::string checkQuery = "CREATE TABLE IF NOT EXISTS Cookies ("
                                    "id IDENTITY PRIMARY KEY,"
                                    "website VARCHAR(255),"
                                    "value VARCHAR(512),"
                                    "timestamp DATE"
                                ")";
    };
} // namespace
