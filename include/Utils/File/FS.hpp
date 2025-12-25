#include <string>

#include <filesystem>
#include <iostream>
#include <fstream>

namespace FS
{
    class FileSystem
    {
    public:
        static void createFile(std::string path);
        static void createFile(std::string path, std::string content);
        static std::string readFile(std::string path);
        static bool deleteFile(std::string path);
    };
} // namespace FS