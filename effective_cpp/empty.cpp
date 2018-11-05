/**
  * Copyright(C), 2017, Nsfocus
  * Name:
  * Author: Wilson Lan
  * Description:
  */

#include <iostream>


class Empty { };


class HoldsAnInt {
    private:
        int num;

        /* According to forced alignment regulation,
         * Empty class may be added with some paddings.
         */
        Empty e;
};

class HoldsAnIntII : private Empty {
    private:
        int num;
};


int main() {
    std::cout << sizeof(Empty) << std::endl;
    std::cout << sizeof(HoldsAnInt) << std::endl;
    std::cout << sizeof(HoldsAnIntII) << std::endl;

    return 0;
}
