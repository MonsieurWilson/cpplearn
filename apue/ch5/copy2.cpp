/**
  * Copyright(C), 2017, Nsfocus
  * Name:
  * Author: Wilson Lan
  * Description:
  */

#include <cstdio>
#include <iostream>

#define BUF_SIZE 256


int main() {
    char buf[BUF_SIZE];

    while (fgets(buf, BUF_SIZE, stdin) != NULL) {
        if (fputs(buf, stdout) == EOF) {
            std::cerr << "Fputs error" << std::endl;
            return -1;
        }
    }

    if (ferror(stdin)) {
        std::cerr << "Fgets error" << std::endl;
        return -1;
    }

    return 0;
}
