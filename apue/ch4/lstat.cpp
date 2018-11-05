/**
  * Copyright(C), 2017, Nsfocus
  * Name:
  * Author: Wilson Lan
  * Description:
  */

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>


int main(int argc, const char **argv) {
    struct stat buf;
    std::string mode;

    for (int i = 1; i < argc; ++i) {
        std::cout << argv[i] << ": ";
        if (lstat(argv[i], &buf) < 0) {
            std::cerr << "lstat error" << std::endl;
            continue;
        }

        if (S_ISREG(buf.st_mode)) {
            mode = "regular";
        } else if (S_ISDIR(buf.st_mode)) {
            mode = "directory";
        } else if (S_ISCHR(buf.st_mode)) {
            mode = "character special";
        } else if (S_ISBLK(buf.st_mode)) {
            mode = "block special";
        } else if (S_ISFIFO(buf.st_mode)) {
            mode = "fifo";
        } else if (S_ISSOCK(buf.st_mode)) {
            mode = "socket";
        } else {
            mode = "** unknown mode **";
        }
        
        std::cout << mode << std::endl;
    }

    return 0;
}
