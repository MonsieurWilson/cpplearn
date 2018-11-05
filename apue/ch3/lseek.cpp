/**
  * Copyright(C), 2017, Nsfocus
  * Name:
  * Author: Wilson Lan
  * Description:
  */

#include <unistd.h>
#include <fcntl.h>
#include <cerrno>
#include <cstring>
#include <iostream>


int main(int argc, const char **argv) {
    int fd;
    int returncode = -1;

    if (argc != 2) {
        std::cerr << "lseek <file>" << std::endl;
        return returncode;
    }

    if (-1 == (fd = open(argv[1], O_RDONLY))) {
        std::cerr << "Open file error: " << strerror(errno) << std::endl;
        return returncode;
    }

    if (-1 == lseek(fd, 0 ,SEEK_CUR)) {
        std::cerr << "Can't seek" << std::endl;
    } else {
        std::cout << "Seek OK" << std::endl;
        returncode = 0;
    }

    return returncode;
}
