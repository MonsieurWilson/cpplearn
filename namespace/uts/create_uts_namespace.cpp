/**
  * Copyright(C), 2017, Nsfocus
  * Name:
  * Author: Wilson Lan
  * Description:
  */

#include <sched.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#include <cerrno>
#include <cstring>
#include <iostream>


static const char *PROGRAM_NAME = "create_uts_namespace";
static char CHILD_STACK[1024 * 1024];

static int child_func(void *hostname) {
    const char *hn = static_cast<const char *>(hostname);

    /* set hostname */
    sethostname(hn, strlen(hn));

    /* execute a new bash to substitute current process */
    execlp("bash", "bash", (char *)NULL);

    /* the code won't be executed, just leave a return to let compiler happy */
    return 0;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cout << "Usage: " << PROGRAM_NAME 
                  << " <new hostname>" << std::endl;
        return 1;
    }

    pid_t pid = clone(child_func,
                      CHILD_STACK+sizeof(CHILD_STACK),
                      CLONE_NEWUTS|SIGCHLD,
                      argv[1]);
    if (pid == -1) {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        return 1;
    }

    /* wait for child process */
    if (waitpid(pid, NULL, 0) == -1) {
        std::cerr << "Error: waitpid error" << std::endl;
        return 1;
    }


    return 0;
}
