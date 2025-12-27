#pragma once

#include <string>

#include <filesystem>
#include <iostream>
#include <fstream>

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

namespace FS
{
    class FileSystem
    {
    public:
        static void createFile(std::string path);
        static void createFile(std::string path, std::string content);
        static std::string readFile(std::string path);
        static bool deleteFile(std::string path);

        static bool createFileWithPerms(std::string path);
        static bool createFileWithPerms(std::string path, std::string content);
        
        static bool isExistFolder(std::string path);
        static bool isExistFile(std::string path);
    };
} // namespace FS