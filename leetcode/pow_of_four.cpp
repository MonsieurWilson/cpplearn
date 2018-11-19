/**
  * Copyright(C), 2018
  * Name: pow_of_four
  * Author: Wilson Lan
  * Description:
  *     Given an integer (signed 32 bits), write a function to check
  *     whether it is a power of 4.
  */

class Solution {
    public:
        bool isPowerOfFour(int num) {
            if (num < 1) {
                return false;
            }
            while (num % 4 == 0) {
                num /= 4;
            }
            return num == 1;
        }
};
