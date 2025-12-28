#pragma once

#include <sys/un.h>

#include <filesystem>
#include <iostream>
#include <fstream>

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#include "Core/IPC/Protocol.hpp"

namespace Storage
{
    constexpr const char *SANDBOX_ROOT = "/home/void/";

    class FileStore
    {
    public:
        inline static std::string BASE_PATH = std::string(SANDBOX_ROOT) + "/.local/share/void-browser/";

        static int readFile(
            const char *rel_path,
            uint8_t *out_data,
            uint32_t *out_len);

        static int writeFile(
            const char *rel_path,
            const uint8_t *data,
            uint32_t len);

        static void mkdirRecursive(const std::string &path);
    };
} // namespace IPC