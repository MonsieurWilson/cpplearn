/**
  * Copyright(C), 2017, Nsfocus
  * Name:
  * Author: Wilson Lan
  * Description:
  */

#include <sys/wait.h>
#include <unistd.h>
#include <cstdlib>
#include <iostream>


void pr_exit(int st);

int main() {
    int   st;
    pid_t pid;

    if ((pid = fork()) < 0) {
        std::cerr << "fork error" << std::endl;
    } else if (pid == 0) {
        /* child process */
        exit(7);
    }

    if (wait(&st) != pid) {
        std::cerr << "wait error" << std::endl;
    }
    pr_exit(st);

    if ((pid = fork()) < 0) {
        std::cerr << "fork error" << std::endl;
    } else if (pid == 0) {
        /* child process */
        abort();
    }

    if (wait(&st) != pid) {
        std::cerr << "wait error" << std::endl;
    }
    pr_exit(st);

    if ((pid = fork()) < 0) {
        std::cerr << "fork error" << std::endl;
    } else if (pid == 0) {
        /* child process */
        st /= 0;
    }

    if (wait(&st) != pid) {
        std::cerr << "wait error" << std::endl;
    }
    pr_exit(st);

    return 0;
}

void pr_exit(int st) {
    if (WIFEXITED(st)) {
        std::cout << "normal termination, exit status = " 
                  << WEXITSTATUS(st) << std::endl;
    } else if (WIFSIGNALED(st)) {
        std::cout << "abnormal termination, signal number = "
                  << WTERMSIG(st) << std::endl;
    } else if (WIFSTOPPED(st)) {
        std::cout << "child stopped, signal number = " 
                  << WSTOPSIG(st) << std::endl;
    }
}
