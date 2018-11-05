/**
  * Copyright(C), 2017, Nsfocus
  * Name:
  * Author: Wilson Lan
  * Description:
  */

#include <sched.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>

#include <cerrno>
#include <cstring>
#include <iostream>


static const size_t BUF_SIZE = 128;
static const char *PROGRAM_NAME = "join_uts_namespace";

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cout << "Usage: " << PROGRAM_NAME
                  << " <attached namespace pid>" << std::endl;
        return 1;
    }

    char uts_ns_file[BUF_SIZE];
    snprintf(uts_ns_file, BUF_SIZE, "/proc/%s/ns/uts", argv[1]);

    int fd = open(uts_ns_file, O_RDONLY);
    if (fd == -1) {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        return 1;
    }

    /* join a namespace specified by the file descriptor */
    if (setns(fd, 0) == -1) {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        return 1;
    }

    /* execute a new bash to substitute current process */
    execlp("bash", "bash", (char *)NULL);
        
    return 0;
}
