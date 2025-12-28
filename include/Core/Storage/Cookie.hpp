#pragma once

#include "Core/SQLite/SQLite.hpp"

namespace Storage
{
    class Cookie
    {
    public:
        Cookie();
        void writeCookie();
        void readCookie();
    };
} // namespace IPC
