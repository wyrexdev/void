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

    bool FileSystem::createFileWithPerms(std::string path)
    {
        int fd = open(
            path.c_str(),
            O_CREAT | O_WRONLY | O_TRUNC | O_NOFOLLOW,
            0600);

        if (fd == -1)
        {
            perror("open");
            return false;
        }

        close(fd);

        return true;
    }

    bool FileSystem::createFileWithPerms(std::string path, std::string content)
    {
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
            std::cout << e.what() << std::endl;
            return false;
        }
    }

    bool FileSystem::isExistFile(std::string path)
    {
        struct stat st;
        return stat(path.c_str(), &st) == 0 && S_ISREG(st.st_mode);
    }

    bool FileSystem::isExistFolder(std::string path)
    {
        struct stat st;
        return stat(path.c_str(), &st) == 0 && S_ISDIR(st.st_mode);
    }
} // namespace FS