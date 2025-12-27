#include "Core/Storage/FileStore.hpp"

namespace IPC
{
    int FileStore::readFile(
        const char *rel_path,
        uint8_t *out_data,
        uint32_t *out_len)
    {
        if (strstr(rel_path, "..") || rel_path[0] == '/')
        {
            return -EACCES;
        }

        char full_path[512];
        snprintf(full_path, sizeof(full_path),
                 "%s/%s", SANDBOX_ROOT, rel_path);

        int fd = open(full_path, O_RDONLY | O_NOFOLLOW);
        if (fd < 0)
        {
            return -errno;
        }

        struct stat st;
        if (fstat(fd, &st) != 0)
        {
            close(fd);
            return -errno;
        }

        if (!S_ISREG(st.st_mode))
        {
            close(fd);
            return -EACCES;
        }

        if (st.st_size > MAX_DATA_LEN)
        {
            close(fd);
            return -EFBIG;
        }

        ssize_t r = read(fd, out_data, st.st_size);
        if (r < 0)
        {
            close(fd);
            return -errno;
        }

        *out_len = static_cast<uint32_t>(r);

        close(fd);
        return 0;
    }

    int FileStore::writeFile(
        const char *rel_path,
        const uint8_t *data,
        uint32_t len)
    {
        if (!rel_path || !data)
            return -EINVAL;

        if (len > MAX_DATA_LEN)
            return -EFBIG;

        if (strstr(rel_path, "..") || rel_path[0] == '/')
            return -EACCES;

        char full_path[512];
        snprintf(full_path, sizeof(full_path),
                 "%s/%s", SANDBOX_ROOT, rel_path);

        int fd = open(
            full_path,
            O_WRONLY | O_CREAT | O_TRUNC | O_NOFOLLOW | O_CLOEXEC,
            0600);

        if (fd < 0)
            return -errno;

        struct stat st;
        if (fstat(fd, &st) != 0)
        {
            close(fd);
            return -errno;
        }

        if (!S_ISREG(st.st_mode))
        {
            close(fd);
            return -EACCES;
        }

        ssize_t written = 0;
        while (written < (ssize_t)len)
        {
            ssize_t w = write(fd, data + written, len - written);
            if (w < 0)
            {
                close(fd);
                return -errno;
            }
            written += w;
        }

        close(fd);
        return 0;
    }

} // namespace IPC
