/**
  * Copyright(C), 2017, Nsfocus
  * Name:
  * Author: Wilson Lan
  * Description:
  */

#include <cstdio>
#include <iostream>


int main() {
    int c;

    while ((c = getc(stdin)) != EOF) {
        if (putc(c, stdout) == EOF) {
            std::cerr << "Putc error" << std::endl;
            return -1;
        }
    }

    if (ferror(stdin)) {
        std::cerr << "Getc error" << std::endl;
        return -1;
    }

    return 0;
}
