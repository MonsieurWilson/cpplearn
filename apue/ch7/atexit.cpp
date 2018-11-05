/**
  * Copyright(C), 2017, Nsfocus
  * Name:
  * Author: Wilson Lan
  * Description:
  */

#include <cstdlib>
#include <iostream>


static void my_exit1();
static void my_exit2();

int main() {
    if (atexit(my_exit1) != 0) {
        std::cerr << "can't register my_exit1" << std::endl;
        return -1;
    }

    if (atexit(my_exit2) != 0) {
        std::cerr << "can't register my_exit2" << std::endl;
        return -1;
    }

    if (atexit(my_exit2) != 0) {
        std::cerr << "can't register my_exit2" << std::endl;
        return -1;
    }

    std::cout << "main exit" << std::endl;
    return 0;
}

static void my_exit1() {
    std::cout << "first exit handler" << std::endl;
}

static void my_exit2() {
    std::cout << "second exit handler" << std::endl;
}

