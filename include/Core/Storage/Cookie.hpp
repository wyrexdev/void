#pragma once

#include <sqlite3.h>

namespace IPC
{
    class Cookie
    {
    public:
        Cookie();
        void writeCookie();
        void readCookie();

    private:
        int exit = 0;

        sqlite3 *db;
    };
} // namespace IPC
