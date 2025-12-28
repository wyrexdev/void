#include "Core/Storage/Cookie.hpp"

namespace IPC
{
    Cookie::Cookie() {
        exit = sqlite3_open("", &db);
    }
}