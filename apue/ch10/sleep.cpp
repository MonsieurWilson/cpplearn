/**
  * Copyright(C), 2017, Nsfocus
  * Name:
  * Author: Wilson Lan
  * Description:
  */

#include <setjmp.h>
#include <unistd.h>
#include <signal.h>
#include <iostream>


static jmp_buf env_alrm;

static void sig_alrm1(int signo) {
    /* nothing to do, just return to wake up `pause` */
}

static void sig_alrm2(int signo) {
    longjmp(env_alrm, 1);
}

static void sig_int(int signo) {
    volatile int k;
    std::cout << std::endl << "SIGINT starting" << std::endl;
    for (int i = 0; i < 300000; ++i) {
        for (int j = 0; j < 4000; ++j) {
            k += i * j;
        }
    }
    std::cout << "SIGINT finished" << std::endl;
}

unsigned int sleep1(unsigned int nsecs) {
    if (signal(SIGALRM, sig_alrm1) == SIG_ERR) {
        return nsecs;
    }
    alarm(nsecs);
    pause();
    return alarm(0);
}

unsigned int sleep2(unsigned int nsecs) {
    if (signal(SIGALRM, sig_alrm2) == SIG_ERR) {
        return nsecs;
    }
    if (setjmp(env_alrm) == 0) {
        alarm(nsecs);
        pause();
    }
    return alarm(0);
}

int main() {
    unsigned int unslept;

    if (signal(SIGINT, sig_int) == SIG_ERR) {
        std::cerr << "signal error" << std::endl;
        return -1;
    }
    
    /* sleep2 will interrupt SIGINT signal handle function */
    unslept = sleep2(2);
    std::cout << "sleep2 returned: " << unslept << std::endl;

    return 0;
}
