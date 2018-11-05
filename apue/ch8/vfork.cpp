/**
  * Copyright(C), 2017, Nsfocus
  * Name:
  * Author: Wilson Lan
  * Description:
  */

#include <unistd.h>
#include <cstdio>
#include <iostream>


int glob = 6;

int main() {
    int   var = 88;
    pid_t pid;

    printf("Before vfork\n");
    if ((pid = vfork()) < 0) {
        std::cerr << "vfork error" << std::endl;
        return -1;
    } else if (pid == 0) {
        /* child process */
        ++glob;
        ++var;
        _exit(0);
    }

    /* father process runs after child process */
    std::cout << "pid = "  << getpid() << ", "
              << "glob = " << glob     << ", "
              << "var = "  << var      << std::endl;

    return 0;
}
