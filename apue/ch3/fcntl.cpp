/**
  * Copyright(C), 2017, Nsfocus
  * Name:
  * Author: Wilson Lan
  * Description:
  */

#include <fcntl.h>
#include <unistd.h>
#include <cstdlib>
#include <iostream>


int main(int argc, const char **argv) {
    int returncode = -1;
    int val;

    if (argc != 2) {
        std::cerr << "Usage: fcntl <descriptor>" << std::endl;
        return returncode;
    }

    if ((val = fcntl(atoi(argv[1]), F_GETFL, 0)) < 0) {
        std::cerr << "Fcntl error for fd " << argv[1] << std::endl;
        return returncode;
    }

    switch (val & O_ACCMODE) {
        case O_RDONLY:
            std::cout << "Read only";
            break;
        case O_WRONLY:
            std::cout << "Write only";
            break;
        case O_RDWR:
            std::cout << "Read write";
            break;
        default:
            std::cerr << "Unkown access mode";
            break;
    }

    if (val & O_APPEND) {
        std::cout << ", append";
    }
    if (val & O_NONBLOCK) {
        std::cout << ", nonblocking";
    }
#if defined(O_SYNC)
    if (val & O_SYNC) {
        std::cout << ", synchronous writes";
    }
#endif
    std::cout << std::endl;

    return 0;

    /*
     * Examples:
     * $ fcntl 0 < /dev/tty
     * Read only
     * $ fcntl 1 > tempfile
     * $ cat tempfile
     * Write only
     * # Open tempfile at file descriptor "2" for appending
     * $ fcntl 2 2>>tempfile
     * Write only, append
     * # Open tempfile at file descriptor "5" for read-write
     * $ fcntl 5 5<>tempfile
     * Read write
     *
     */
}
