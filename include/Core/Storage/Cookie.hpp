#pragma once

#include "Core/Storage/FileStore.hpp"

#include <sqlite3.h>

namespace Storage
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
