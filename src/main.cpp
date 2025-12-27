#include "Headers/Global.hpp"

#include "QT/MainWindow.hpp"
#include "QT/Widget/Layouts/Nav.hpp"

#include "System/Setup/Setup.hpp"
#include "System/User/User.hpp"

#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdexcept>
#include <sys/prctl.h>


#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <cstdio>
#include <cstdlib>


int main(int argc, char *argv[])
{
    umask(0077);

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(4, 5);
    format.setSamples(8);
    format.setProfile(QSurfaceFormat::CompatibilityProfile);
    format.setRenderableType(QSurfaceFormat::OpenGL);
    QSurfaceFormat::setDefaultFormat(format);

    QApplication app(argc, argv);

    System::Setup setup;
    QT::MainWindow window;

    if (setup.isSetupNeeded())
    {
    }
    else
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            auto ids = System::User::getUserIds();
            setgid(ids.gid);
            setuid(ids.uid);

            prctl(PR_SET_NO_NEW_PRIVS, 1, 0, 0, 0);
            setsid();
        }
    }

    std::cout << System::User::getCurrentUser() << std::endl;

    window.setStyleSheet(
        "background-color: " + QString::fromStdString(Theme::style.background) + ";");
    window.setWindowTitle("Void Browser");
    window.showMaximized();

    return app.exec();
}

enum class Command : uint32_t {
    INVALID     = 0,

    READ_FILE   = 1,
    WRITE_FILE  = 2,
    DELETE_FILE = 3,

    READ_COOKIE = 10,
    WRITE_COOKIE = 11,

    PING        = 100,
    SHUTDOWN    = 200
};

constexpr size_t MAX_PATH_LEN  = 256;
constexpr size_t MAX_DATA_LEN  = 4096;

struct Request {
    uint32_t magic;
    Command  cmd;
    uint32_t data_len;
    char     path[MAX_PATH_LEN];
    uint8_t  data[MAX_DATA_LEN];
};

#define VOID_MAGIC 0x564F4944

struct Response {
    int32_t  status;
    uint32_t data_len;
    uint8_t  data[MAX_DATA_LEN];
};

void sandboxMain()
{
    int server = socket(AF_UNIX, SOCK_STREAM, 0);

    sockaddr_un addr{};
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, "/var/run/void.sock");

    unlink(addr.sun_path);
    bind(server, (sockaddr *)&addr, sizeof(addr));
    listen(server, 5);

    while (true)
    {
        int client = accept(server, nullptr, nullptr);

        Request req;
        read(client, &req, sizeof(req));

        Response res{};
        if (req.cmd == Command::READ_FILE)
        {
            res.status = readFile(req.path, res.data, &res.data_len);
        }

        write(client, &res, sizeof(res));
        close(client);
    }
}

constexpr const char* SANDBOX_ROOT = "/var/lib/void";

int readFile(
    const char* rel_path,
    uint8_t* out_data,
    uint32_t* out_len
) {
    if (strstr(rel_path, "..") || rel_path[0] == '/') {
        return -EACCES;
    }

    char full_path[512];
    snprintf(full_path, sizeof(full_path),
             "%s/%s", SANDBOX_ROOT, rel_path);

    int fd = open(full_path, O_RDONLY | O_NOFOLLOW);
    if (fd < 0) {
        return -errno;
    }

    struct stat st;
    if (fstat(fd, &st) != 0) {
        close(fd);
        return -errno;
    }

    if (!S_ISREG(st.st_mode)) {
        close(fd);
        return -EACCES;
    }

    if (st.st_size > MAX_DATA_LEN) {
        close(fd);
        return -EFBIG;
    }

    ssize_t r = read(fd, out_data, st.st_size);
    if (r < 0) {
        close(fd);
        return -errno;
    }

    *out_len = static_cast<uint32_t>(r);

    close(fd);
    return 0;
}