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


int main() {
    pid_t pid;

    if ((pid = fork()) < 0) {
        std::cerr << "fork #1 error" << std::endl;
    } else if (pid == 0) {
        /* first child */
        if ((pid = fork()) < 0) {
            std::cerr << "fork #2 error" << std::endl;
        } else if (pid > 0) {
            /* parent from second fork */
            exit(0);
        }

        /*
         * We're the second child, our parent become init as soon
         * as our real parent calls exit() in the statement above.
         * Here's where we'd continue executing, knowing that when
         * we're done, init will reap our status.
         */
        sleep(2);
        std::cout << "second child, parent pid = " << getppid() 
                  << std::endl;
        exit(0);
    }

    if (waitpid(pid, NULL, 0) != pid) {
        /* wait for the first child */
        std::cerr << "waitpid error" << std::endl;
    }

    /*
     * We're the original process and going to exit knowing that
     * we're not the parent of the second child.
     */
    return 0;
}
