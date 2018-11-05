/**
  * Copyright(C), 2017, Nsfocus
  * Name:
  * Author: Wilson Lan
  * Description:
  */

#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <iostream>


int main() {
    int fd;
    int returncode = 0;
    const char *filename = "/tmp/testfile";
    const char *file_contents = "Hello World\n";

    if (-1 == (fd = open(filename, O_WRONLY|O_CREAT|O_EXCL))) {
        std::cerr << "File exists." << std::endl;
        returncode = -1;
    } else {
        ssize_t nbytes = write(fd, file_contents, strlen(file_contents));
        if (nbytes != strlen(file_contents)) {
            std::cerr << "Write error." << std::endl;
            returncode = -1;
        }
    }

    return returncode;
}
