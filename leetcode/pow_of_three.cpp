/**
  * Copyright(C), 2018
  * Name: pow_of_three
  * Author: Wilson Lan
  * Description:
  *     Given an integer, write a function to determine
  *     if it is a power of three.
  */

class Solution {
    public:
        bool isPowerOfThree(int n) {
            if (n < 1) {
                return false;
            }
            while (n % 3 == 0) {
                n /= 3;
            }
            return n == 1;
        }
};
