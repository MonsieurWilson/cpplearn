/**
  * Copyright(C), 2017, Nsfocus
  * Name:
  * Author: Wilson Lan
  * Description:
  */

#include <unistd.h>
#include <signal.h>
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <iostream>


static void sig_hup(int signo) {
    std::cout << "SIGHUP received, pid = " << getpid() << std::endl;
}

static void pr_ids(const char *name) {
    std::cout << name << ": "
              << "pid = "   << getpid() << ", "
              << "ppid = "  << getppid() << ", "
              << "pgrp = "  << getpgrp() << ", "
              << "tpgrp = " << tcgetpgrp(STDIN_FILENO) << std::endl;
}

int main() {
    char c;
    pid_t pid;

    pr_ids("parent");
    if ((pid = fork()) < 0) {
        std::cerr << "fork error" << std::endl;
        return -1;
    } else if (pid > 0) {
        /* father process */
        sleep(5);
        exit(0);
    } else {
        pr_ids("child #1");
        signal(SIGHUP, sig_hup);
        kill(getpid(), SIGTSTP);
        pr_ids("child #2");
        if (read(STDIN_FILENO, &c, 1) != 1) {
            std::cerr << "read error from controlling TTY, errno = " 
                      << errno << ": " << strerror(errno) << std::endl;
        }
        exit(0);
    }
}
