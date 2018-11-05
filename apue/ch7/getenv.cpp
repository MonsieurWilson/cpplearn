/**
  * Copyright(C), 2017, Nsfocus
  * Name:
  * Author: Wilson Lan
  * Description:
  */

#include <cstdlib>
#include <iostream>


int main() {
    std::cout << "PATH: " << getenv("PATH") << std::endl;
    std::cout << "TERM: " << getenv("TERM") << std::endl;

    return 0;
}
