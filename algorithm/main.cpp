/**
  * Copyright(C), 2018, Nsfocus
  * Name:
  * Author: Wilson Lan
  * Description:
  */

#include <vector>
#include <iostream>
#include <sstream>
#include <cassert>
#include <ctime>
#include <cstdlib>

#include "qsort.h"
#include "qselect.h"


template <typename ElementType>
std::istream &operator>>(std::istream &ins,
                         std::vector<ElementType> &nums) {
    ElementType num;
    while (ins >> num) {
        nums.push_back(num);
    }
    return ins;
}

template <typename ElementType>
std::ostream &operator<<(std::ostream &out,
                         const std::vector<ElementType> &nums) {
    for (const auto &num : nums) {
        out << num << " ";
    }
    return out;
}

int main() {
    /*
    std::string line;
    std::cout << "Input numbers to be sorted:" << std::endl;
    while (std::getline(std::cin, line)) {
        std::istringstream iss;
        std::vector<int> nums;
        iss.str(line);
        iss >> nums;
        lxy::qsort(nums);
        std::cout << "After quick sort:" << std::endl 
                  << nums << std::endl;
    }
    */

    std::string line;
    std::cout << "Input numbers to be selected:" << std::endl;
    while (std::getline(std::cin, line)) {
        std::istringstream iss;
        std::vector<int> nums;
        iss.str(line);
        iss >> nums;

        srand(static_cast<int>(time(0)));
        typename std::vector<int>::size_type pos;
        if (nums.size() == 0) {
            pos = 1;
        } else {
            pos = rand() % nums.size() + 1;
        }

        auto rs = lxy::randomized_select(nums, pos),
             hs = lxy::heap_select(nums, pos),
             moms = lxy::median_of_median_select(nums, pos);
        assert(rs == hs);
        assert(moms == hs);
        std::cout << "============Passed===========" << std::endl;
    }
}
