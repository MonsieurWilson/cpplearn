/**
  * Copyright(C), 2017
  * Name: excercise_16_19.cpp
  * Author: Wilson Lan
  * Description:
  */

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <deque>

template <typename T>
void dump_container(const T& c) {
    /*
     * This case, `using` and `typedef` both work well.
     *     using size_type = typename T::size_type;
     *     typedef typename T::size_type size_type;
     */
    typedef typename T::size_type size_type;
    for (size_type idx = 0; idx != c.size(); ++idx) {
        std::cout << c[idx] << " ";
    }
}

int main() {
    std::vector<std::string> strs = {"hello", "world"};
    std::deque<double> dbs = {1.5, 2.5, 3.5, 4.5, 5.5};
    std::array<int, 5> ints = {1, 2, 3, 4, 5};

    dump_container(strs);
    std::cout << std::endl;
    dump_container(dbs);
    std::cout << std::endl;
    dump_container(ints);
    std::cout << std::endl;

    return 0;
}
