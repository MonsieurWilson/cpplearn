/**
  * Copyright(C), 2017, Nsfocus
  * Name:
  * Author: Wilson Lan
  * Description:
  */

#include <unistd.h>
#include <sys/wait.h>
#include <cerrno>
#include <iostream>


int system(const char *cmd) {
    pid_t pid;
    int   st;

    if (cmd == NULL) {
        return 1;
    }

    if ((pid = fork()) < 0) {
        st = -1;
    } else if (pid == 0) {
        /*
        if (execl("/bin/sh", "sh", "-c", cmd, (char *)0) < 0) {
            _exit(127);
        }
        */
        execl("/bin/sh", "sh", "-c", cmd, (char *)0);
        _exit(127);
    } else {
        while (waitpid(pid, &st, 0) < 0) {
            if (errno != EINTR) {
                st = -1;
                break;
            }
        }
    }

    return st;
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

int test_exit_code() {
    int st;

    if ((st = system("date")) < 0) {
        std::cerr << "system() error" << std::endl;
        return -1;
    }
    pr_exit(st);

    if ((st = system("nosuchcommand")) < 0) {
        std::cerr << "system() error" << std::endl;
        return -1;
    }
    pr_exit(st);

    if ((st = system("who; exit 44")) < 0) {
        std::cerr << "system() error" << std::endl;
        return -1;
    }
    pr_exit(st);

    return 0;
}

int main(int argc, const char **argv) {
    int st;

    if (argc != 2) {
        std::cerr << "system <command>" << std::endl;
        return -1;
    }

    if ((st = system(argv[1])) < 0) {
        std::cerr << "system() error" << std::endl;
        return -1;
    }
    pr_exit(st);

    return 0;
}
