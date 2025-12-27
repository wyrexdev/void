#pragma once

#include <cinttypes>
#include <cstdio>

namespace IPC
{
    constexpr size_t MAX_PATH_LEN = 256;
    constexpr size_t MAX_DATA_LEN = 4096;

    enum class Command : uint32_t
    {
        INVALID = 0,

        READ_FILE = 1,
        WRITE_FILE = 2,
        DELETE_FILE = 3,

        READ_COOKIE = 10,
        WRITE_COOKIE = 11,

        PING = 100,
        SHUTDOWN = 200
    };

    class Protocol
    {
    public:
        struct Request
        {
            uint32_t magic;
            Command cmd;
            uint32_t data_len;
            char path[MAX_PATH_LEN];
            uint8_t data[MAX_DATA_LEN];
        };

        struct Response
        {
            int32_t status;
            uint32_t data_len;
            uint8_t data[MAX_DATA_LEN];
        };
    };

} // namespace IPC