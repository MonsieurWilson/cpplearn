#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>

#include <cstddef>
#include <cerrno>
#include <cstring>

#include "unix_socket.h"
#include "log.h"
#include "utils.h"


int unix_serv_listen(const char *sock_name) {
    int fd, len ,err, ret;
    struct sockaddr_un sock_un;
    const int max_queue_len = 10;

    if ((fd = socket(AF_LOCAL, SOCK_STREAM, 0)) < 0) {
        return -1;
    }
    /* unlink in advance in case that the sock file exits */
    unlink(sock_name);

    memset(&sock_un, 0, sizeof(sock_un));
    sock_un.sun_family = AF_LOCAL;
    strcpy(sock_un.sun_path, sock_name);
    len = offsetof(struct sockaddr_un, sun_path) + strlen(sock_name);

    if (bind(fd, (struct sockaddr *)&sock_un, len) < 0) {
        ret = -2;
        goto errout;
    }

    if (listen(fd, max_queue_len) < 0) {
        ret = -3;
        goto errout;
    }

    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);
    return fd;

errout:
    /* ignore the errno of close operation */
    err = errno;
    close(fd);
    errno = err;

    return ret;
}

int unix_serv_accept(int listen_fd) {
    int fd, err, ret;
    socklen_t len;
    struct sockaddr_un sock_un;
    struct stat stat_buf;
    time_t stale_time;

    len = sizeof(sock_un);
    while ((fd = accept(listen_fd, (struct sockaddr *)&sock_un, &len)) < 0) {
        if (EINTR == errno) {
            continue;
        } else {
            return -1;
        }
    }

    len -= offsetof(struct sockaddr_un, sun_path);
    sock_un.sun_path[len] = 0;
    
    if (stat(sock_un.sun_path, &stat_buf) < 0) {
        ret = -2;
        goto errout;
    }

    if (((S_IRWXG | S_IRWXO) & stat_buf.st_mode) ||
        S_IRWXU != (stat_buf.st_mode & S_IRWXU)) {
        /* is not rwx------ */
        ret = -3;
        goto errout;
    }

    /* client's name can't be older than 30 seconds */
    stale_time = time(NULL) - 30;
    if (stat_buf.st_atime < stale_time ||
        stat_buf.st_ctime < stale_time ||
        stat_buf.st_mtime < stale_time) {
        ret = -4;
        goto errout;
    }

    unlink(sock_un.sun_path);
    return fd;

errout:
    /* ignore the errno of close operation */
    err = errno;
    close(fd);
    errno = err;

    return ret;
}

int unix_cli_connect(const char *sock_name) {
    int fd, len, err, ret;
    struct sockaddr_un sock_un;

    if ((fd = socket(AF_LOCAL, SOCK_STREAM, 0)) < 0) {
        return -1;
    }

    memset(&sock_un, 0, sizeof(sock_un));
    sock_un.sun_family = AF_LOCAL;
    sprintf(sock_un.sun_path, "/var/tmp/%05d.sock", getpid());
    len = offsetof(struct sockaddr_un, sun_path) + \
          strlen(sock_un.sun_path);
    /* in case it already exists */
    unlink(sock_un.sun_path);

    if (bind(fd, (struct sockaddr *)&sock_un, len) < 0) {
        ret = -2;
        goto errout;
    }

    if (chmod(sock_un.sun_path, S_IRWXU) < 0) {
        ret = -3;
        goto errout;
    }

    memset(&sock_un, 0, sizeof(sock_un));
    sock_un.sun_family = AF_LOCAL;
    strcpy(sock_un.sun_path, sock_name);
    len = offsetof(struct sockaddr_un, sun_path) + strlen(sock_name);

    if (connect(fd, (struct sockaddr *)&sock_un, len) < 0) {
        ret = -4;
        goto errout;
    }

    return fd;

errout:
    /* ignore the errno of close operation */
    err = errno;
    close(fd);
    errno = err;

    return ret;
}

