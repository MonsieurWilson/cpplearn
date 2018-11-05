
#include "misc.h"


/* Thread start function: display address near top of our stack,
   and return upper-cased copy of argv_string */

void * xdf::thread_start(void *arg)
{
    ThreadInfo *tinfo = (ThreadInfo *)arg;
    tinfo->func(tinfo->arg);
    delete tinfo;
    return 0;
}

/**
 *  @brief ´´½¨Ïß³Ì
 *
 *  @param func  º¯ÊýµØÖ·
 *  @param arg   º¯Êý²ÎÊý
 *
 *  @return true on success, false on failure
 */
bool xdf::thread_create(void* (*func)(void *), void* arg, bool use_dpdk)
{
#ifdef WIN32
    HANDLE handle = CreateThread(NULL, // Security attribute
                        0,    // stack size
                        (LPTHREAD_START_ROUTINE) func,    // func addr
                        arg,    // arg
                        0,        // creation flag
                        NULL);    // thread id
    if( handle == NULL ) {
        return false;
    }
    CloseHandle(handle);
    return true;
#else
    pthread_t tid;
    pthread_attr_t attr;
    pthread_attr_t *p_attr = NULL;
    ThreadInfo *tinfo = new ThreadInfo;
    tinfo->func = func;
    tinfo->arg = arg;

    if( use_dpdk ) {
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CPU_SET(0, &cpuset);
        pthread_attr_init(&attr);
        pthread_attr_setaffinity_np(&attr, sizeof(cpuset), &cpuset);
        p_attr = &attr;
    }

    int ret = pthread_create(&tid, p_attr, &thread_start, tinfo);
    if( p_attr ) {
        pthread_attr_destroy(p_attr);
    }
    if( ret != 0 ) {
        delete tinfo;
        return false;
    }
    ret = pthread_detach(tid);
    if( ret != 0 ) {
        return false;
    }
    return true;
#endif
}

time_t xdf::strftime_to_timestamp(const char *str_fmt_time, const char *fmt) 
{
    struct tm tv = {0};
    time_t ret;
    strptime(str_fmt_time, fmt, &tv);
    ret = mktime(&tv);
    return ret;
}

char *xdf::timestamp_to_strftime(const time_t ts, char *str_fmt_time, const char *fmt) 
{
    struct tm *tv;
    tv = localtime(&ts);
    strftime(str_fmt_time, 32, fmt, tv);
    return str_fmt_time;
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

static void (*my_signal(int signo, void (*func)(int)))(int) {
    struct sigaction act, oact;

    act.sa_handler = func;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
#ifdef SA_INTERRUPT
    act.sa_flags |= SA_INTERRUPT;
#endif
    if (sigaction(signo, &act, &oact) < 0) {
        return SIG_ERR;
    }
    return oact.sa_handler;
}

bool xdf::singleton(const char *pid_file, void (*sig_handler)(int))
{
    int fd;
    char buf[16];

    if ((fd = open(pid_file, O_RDWR|O_CREAT, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)) < 0 ) {
        fprintf(stderr, "Error: open lock file failed.\n");
        return false;
    }

    if (!lock_file(fd)) {
        fprintf(stderr, "Error: lock_file failed.\n");
        return false;
    }

    ftruncate(fd, 0);
    snprintf(buf, 16, "%ld\n", (long)getpid());
    write(fd, buf, strlen(buf));

    /* register signals */
    my_signal(SIGINT,  sig_handler);
    my_signal(SIGQUIT, sig_handler);
    my_signal(SIGILL,  sig_handler);
    my_signal(SIGABRT, sig_handler);
    my_signal(SIGFPE,  sig_handler);
    my_signal(SIGTERM, sig_handler);
    my_signal(SIGHUP,  sig_handler);
    my_signal(SIGCHLD, sig_handler);
    my_signal(SIGSEGV, sig_handler);

    return true;
}

bool xdf::is_different_day(time_t t1, time_t t2)
{
    struct tm tm1, tm2;
    localtime_r(&t1, &tm1);
    localtime_r(&t2, &tm2);
    return (tm1.tm_year != tm2.tm_year || 
            tm1.tm_mon != tm2.tm_mon || 
            tm1.tm_mday != tm2.tm_mday);
}

