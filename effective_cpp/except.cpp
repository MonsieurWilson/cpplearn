/**
  * Copyright(C), 2017, Nsfocus
  * Name:
  * Author: Wilson Lan
  * Description:
  */

#include <iostream>
#include <stdexcept>


void old_style() throw();
void new_style() noexcept(true);


int main() {
    try {
        old_style();
    } catch (...) {
        std::cerr << "Exception #1 caught: " << std::endl;
    }

    try {
        new_style();
    } catch (...) {
        std::cerr << "Exception #2 caught: " << std::endl;
    }

    return 0;
}

void old_style() throw() {
    throw std::runtime_error("old style exception");
}

void new_style() noexcept(true) {
    throw std::runtime_error("new style exception");
}


