/**
  * Copyright(C), 2017, Nsfocus
  * Name:
  * Author: Wilson Lan
  * Description:
  */

#include <memory>
#include <iostream>
#include <tr1/memory>


int main() {
    /* 
     * Noticing the differences between shared_ptr
     * in C++11 and tr1
     */
    std::shared_ptr<int> iptr(0);
    std::tr1::shared_ptr<int> iptr2(static_cast<int *>(0));
    
    iptr  = std::make_shared<int>(3);
    iptr2 = std::tr1::shared_ptr<int>(new int(3));

    std::cout << *iptr  << std::endl;
    std::cout << *iptr2 << std::endl;

    return 0;
}
