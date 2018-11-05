/**
  * Copyright(C), 2017, Nsfocus
  * Name:
  * Author: Wilson Lan
  * Description:
  */

#include <unistd.h>
#include <cstdio>
#include <iostream>


int  glob = 6;
char buf[] = "a write to stdout\n";

int main() {
    std::ios_base::sync_with_stdio(false);

    pid_t pid;
    int var = 88;

    if (write(STDOUT_FILENO, buf, sizeof(buf)-1) != sizeof(buf)-1) {
        std::cerr << "write error" << std::endl;
        return -1;
    }

    /* std::endl will force flushing stdout */
    std::cout << "Before fork:\n";

    if ((pid = fork()) < 0) {
        std::cerr << "fork error" << std::endl;
        return -1;
    } else if (pid == 0) {
        /* child process */
        ++glob;
        ++var;
    } else {
        /* father process */
        sleep(2);
    }

    std::cout << "pid = "  << pid  << ", "
              << "glob = " << glob << ", "
              << "var = "  << var  << std::endl;

    return 0;
}

