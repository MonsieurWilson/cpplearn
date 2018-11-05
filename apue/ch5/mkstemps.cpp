/**
  * Copyright(C), 2017, Nsfocus
  * Name:
  * Author: Wilson Lan
  * Description:
  */

#include <unistd.h>
#include <errno.h>
#include <cstdlib>
#include <cstring>
#include <iostream>

#define MAX_FILENAME 128


int main(int argc, const char **argv) {
    if (argc != 3) {
        std::cerr << "mkstemps <directory> <suffix>" << std::endl;
        return -1;
    }

    char pattern[MAX_FILENAME];
    const char *directory = argv[1], *suffix = argv[2];
    snprintf(pattern, MAX_FILENAME, "%s/XXXXXX%s", directory, suffix);

    int fp = mkstemps(pattern, strlen(suffix));
    if (fp < 0) {
        std::cerr << "mkstemps error [" << errno << "], " << strerror(errno) << std::endl;
        return -1;
    }

    write(fp, pattern, strlen(pattern));
    close(fp);

    return 0;
}
