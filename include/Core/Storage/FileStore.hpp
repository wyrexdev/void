#pragma once

#include <sys/un.h>

#include <filesystem>
#include <iostream>
#include <fstream>

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#include "Core/IPC/Protocol.hpp"

namespace IPC
{
    constexpr const char *SANDBOX_ROOT = "~/.local/share/void-browser";

    class FileStore
    {
    public:
        static int readFile(
            const char *rel_path,
            uint8_t *out_data,
            uint32_t *out_len);

        int writeFile(
            const char *rel_path,
            const uint8_t *data,
            uint32_t len);
    };
} // namespace IPC