/**
  * Copyright(C), 2017, Nsfocus
  * Name:
  * Author: Wilson Lan
  * Description:
  */

#include <iostream>

#include "queue.h"

int main() {
    lxy::Queue<int> q1, q2;

    q1.push(3);
    q1.push(5);
    q2.push(4);
    
    std::cout << "Before swap:" << std::endl;
    std::cout << q1 << std::endl;
    std::cout << q2 << std::endl;

    using std::swap;
    q2 = q1;

    std::cout << "After assignment:" << std::endl;
    std::cout << q1 << std::endl;
    std::cout << q2 << std::endl;

    return 0;
}
