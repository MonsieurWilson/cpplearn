#include <unistd.h>
#include <uuid/uuid.h>

#include <fcntl.h>
#include <ctime>
#include <cstring>
#include <csignal>

#include "utils.h"


time_t strftime_to_timestamp(const char *str_fmt_time, const char *fmt) {
    struct tm tv = {0};
    time_t ret;
    strptime(str_fmt_time, fmt, &tv);
    ret = mktime(&tv);
    return ret;
}

char *timestamp_to_strftime(const time_t ts, char *str_fmt_time, const char *fmt) {
    struct tm *tv;
    tv = localtime(&ts);
    strftime(str_fmt_time, BUF_SIZE, fmt, tv);
    return str_fmt_time;
}

time_t parquet_file_dir_to_timestamp(char *file_dir) {
    /* For instance,
     *      /snappy/20170901
     *      |      |        
     *   file_dir start
     */
    char *start = strrchr(file_dir, '/');
    return strftime_to_timestamp(++start, "%Y%m%d");
}

void parquet_file_path_to_timestamp(char *file_path, time_t &start_ts, time_t &end_ts) {
    /* For instance,
     *      /snappy/20130619/1371601523-1409777555*.parquet
     *      |               |        
     *   file_dir         start
     */
    char *start;
    start = strrchr(file_path, '/');
    sscanf(++start, "%lu", &start_ts);
    start = strchr(file_path, '-');
    sscanf(++start, "%lu", &end_ts);
}

static inline int lock_reg(int fd, int cmd, int type) 
{
    struct flock flk;
    flk.l_type   = type;
    flk.l_start  = 0;
    flk.l_whence = SEEK_SET;
    flk.l_len    = 0;
    return (fcntl(fd, cmd, &flk));
}

static bool inline lock_file(int fd) 
{
    return lock_reg(fd, F_SETLK, F_WRLCK) < 0 ? false : true;
}

static void inline unlock_file(int fd)
{
    /* If unlock fails, we simply do nothing.
     * Because the main process is going to exit.*/
    lock_reg(fd, F_SETLK, F_UNLCK);
}

void (*my_signal(int signo, void (*func)(int)))(int) {
    struct sigaction act, oact;

    act.sa_handler = func;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    
    if (SIGALRM == signo) {
#ifdef SA_INTERRUPT
        act.sa_flags |= SA_INTERRUPT;
#endif
    } else {
#ifdef SA_RESTART
        act.sa_flags |= SA_RESTART;
#endif
    }
    /* do not transform child(ren) process into zoobies */
    // act.sa_flags |= SA_NOCLDWAIT;
    if (sigaction(signo, &act, &oact) < 0) {
        return SIG_ERR;
    }
    return oact.sa_handler;
}

bool singleton(const char *pid_file, void (*sig_handler)(int))
{
    int fd;
    char buf[BUF_SIZE];

    if ((fd = open(pid_file, O_RDWR|O_CREAT, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)) < 0 ) {
        fprintf(stderr, "ERROR: open lock file failed.\n");
        return false;
    }

    if (!lock_file(fd)) {
        fprintf(stderr, "ERROR: lock_file failed.\n");
        return false;
    }

    ftruncate(fd, 0);
    snprintf(buf, BUF_SIZE, "%ld\n", (long)getpid());
    write(fd, buf, strlen(buf));

    /* register signals */
    my_signal(SIGTERM, sig_handler);
    my_signal(SIGHUP,  sig_handler);
    my_signal(SIGCHLD, sig_handler);
    my_signal(SIGINT,  sig_handler);
    my_signal(SIGQUIT, sig_handler);
    my_signal(SIGILL,  sig_handler);
    my_signal(SIGABRT, sig_handler);
    my_signal(SIGFPE,  sig_handler);
    my_signal(SIGSEGV, sig_handler);

    return true;
}

bool process_exist(const char *pid_file) {
    int fd;
    bool ret = true;
    pid_t pid;
    char buf[BUF_SIZE];

    if (-1 == access(pid_file, F_OK)) {
        return false;
    }

    if (-1 == (fd = open(pid_file, O_RDONLY))) {
        fprintf(stderr, "ERROR: open file failed.\n");
        return false;
    }

    lseek(fd, 0, SEEK_SET);
    read(fd, buf, sizeof(buf));
    pid = atoi(buf);

    if (-1 == kill(pid, 0)) {
        ret = false;
    }

    close(fd);
    return ret;
}

void generate_uuid(char *out) {
    uuid_t uuid;
    uuid_generate(uuid);
    uuid_unparse(uuid, out);
}

ssize_t readn(int fd, void *vptr, size_t n) {
    size_t nleft, nread;
    char *ptr;

    ptr = (char *)vptr;
    nleft = n;
    while (nleft > 0) {
        if ((nread = read(fd, ptr, nleft)) < 0) {
            if (EINTR == errno) {
                /* slow system call, need to restart */
                nread = 0;
            } else {
                return -1;
            }
        } else if (0 == nread) {
            /* EOF */
            break;
        }
        nleft -= nread;
        ptr   += nread;
    }

    return n - nleft;
}

ssize_t writen(int fd, const void *vptr, size_t n) {
    size_t nleft, nwritten;
    const char *ptr;

    ptr = (const char *)vptr;
    nleft = n;
    while (nleft > 0) {
        if ((nwritten = write(fd, ptr, nleft)) <= 0) {
            if (nwritten < 0 && EINTR == errno) {
                /* slow system call, need to restart */
                nwritten = 0;
            } else {
                return -1;
            }
        }
        nleft -= nwritten;
        ptr   += nwritten;
    }

    return n;
}

bool read_sock(int fd, TaskInfo &task_info) {
    SockHeader header;
    char *json_contents;

    /* 1. read UnixSockHeader */
    if (readn(fd, &header, sizeof(header)) <= 0) {
        return false;
    }

    /* 2. read TaskInfo */
    json_contents = (char *)calloc(header.msg_length+1, sizeof(char));
    if (readn(fd, json_contents, header.msg_length) <= 0) {
        return false;
    }

    bool ret = task_info.load(json_contents);

    free(json_contents);

    return ret;
}

bool write_sock(int fd, TaskInfo &task_info) {
    const char *json_contents = task_info.json_string();
    SockHeader header;

    strncpy(header.msg_type, "TaskInfo", BUF_SIZE);
    header.msg_length = strlen(json_contents);

    /* 1. write UnixSockHeader */
    if (writen(fd, &header, sizeof(header)) <= 0) {
        return false;
    }

    /* 2. write TaskInfo */
    if (writen(fd, json_contents, header.msg_length) <= 0) {
        return false;
    }

    return true;
}

