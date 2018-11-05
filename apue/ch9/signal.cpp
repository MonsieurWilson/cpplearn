/**
  * Copyright(C), 2017, Nsfocus
  * Name:
  * Author: Wilson Lan
  * Description:
  */

#include <signal.h>
#include <unistd.h>
#include <iostream>


static void sig_usr(int);

int main() {
    if (signal(SIGUSR1, sig_usr) == SIG_ERR) {
        std::cerr << "can't catch SIGUSR1" << std::endl;
        return -1;
    }

    if (signal(SIGUSR2, sig_usr) == SIG_ERR) {
        std::cerr << "can't catch SIGUSR2" << std::endl;
        return -1;
    }

    for (; ;) {
        pause();
    }

    return 0;
}

static void sig_usr(int signo) {
    if (signo == SIGUSR1) {
        // std::cout << "received SIGUSR1" << std::endl;
        psignal(signo, "received");
    } else if (signo == SIGUSR2) {
        std::cout << "received SIGUSR2" << std::endl;
    } else {
        std::cerr << "received signal " << signo << std::endl;
    }
}
