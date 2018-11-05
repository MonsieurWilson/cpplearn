/**
  * Copyright(C), 2017, Nsfocus
  * Name:
  * Author: Wilson Lan
  * Description:
  */

#include <unistd.h>
#include <iostream>


int main() {
    std::cout << "real uid = "      << getuid()  << ", "
              << "effective uid = " << geteuid() << std::endl;

    return 0;
}
