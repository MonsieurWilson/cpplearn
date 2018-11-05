/**
  * Copyright(C), 2017, Nsfocus
  * Name:
  * Author: Wilson Lan
  * Description:
  */

#include <unistd.h>
#include <sys/wait.h>
#include <iostream>


const char *env_init[] = {
    "USER=unknown",
    "PATH=/tmp",
    NULL
};

int main() {
    pid_t pid;
    if ((pid = fork()) < 0) {
        std::cerr << "fork #1 error" << std::endl;
    } else if (pid == 0) {
        /* first child */
        if (execle("/usr/bin/ls", "ls", "/home", (char *)0, env_init) < 0) {
            std::cerr << "execle error" << std::endl;
        }
    }

    if (waitpid(pid, NULL, 0) < 0) {
        std::cerr << "waitpid error" << std::endl;
    }

    if ((pid = fork()) < 0) {
        std::cerr << "fork #2 error" << std::endl;
    } else if (pid == 0) {
        /* second child */
        if (execlp("ls", "ls", "/usr/local/lib64", (char *)0) < 0) {
            std::cerr << "execlp error" << std::endl;
        }
    }

    /* don't wait for second child */
    return 0;
}
