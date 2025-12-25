#include "Utils/File/FS.hpp"

namespace FS
{
    void FileSystem::createFile(std::string path)
    {
        try
        {
            std::ofstream of(path);
            of << "" << std::endl;
            of.close();
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
    }

    void FileSystem::createFile(std::string path, std::string content)
    {
        try
        {
            std::ofstream of(path);
            of << content << std::endl;
            of.close();
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
    }

    std::string FileSystem::readFile(std::string path)
    {
        try
        {
            std::ifstream f(path);
            std::ostringstream oss;
            oss << f.rdbuf();

            return oss.str();
        }
        catch (const std::exception &e)
        {
            return "";
        }
    }

    bool FileSystem::deleteFile(std::string path)
    {
        try
        {
            std::filesystem::remove(path);
            return true;
        }
        catch (const std::exception &e)
        {
            return false;
        }
    }
} // namespace FS