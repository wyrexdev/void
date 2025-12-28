#include "Core/Storage/Cookie.hpp"

namespace Storage
{
    Cookie::Cookie()
    {
        SQLite::SQLite *sqlite = new SQLite::SQLite();
        sqlite->checkAndCreateTables();
    }
}