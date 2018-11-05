/**
  * Copyright(C), 2017, Nsfocus
  * Name:
  * Author: Wilson Lan
  * Description:
  */

#include <cstdio>
#include <iostream>


int main(int argc, const char **argv) {
    if (argc != 3) {
        std::cerr << "tempnam <directory> <prefix>" << std::endl;
        return -1;
    }

    std::cout << tempnam(argv[1][0] != ' ' ? argv[1] : NULL, 
                         argv[2][0] != ' ' ? argv[2] : NULL) << std::endl;
    return 0;
}
