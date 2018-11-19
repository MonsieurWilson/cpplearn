/**
  * Copyright(C), 2018
  * Name: pow_of_two
  * Author: Wilson Lan
  * Description:
  *     Given an integer, write a function to determine
  *     if it is a power of two.
  */

class Solution {
    public:
        bool isPowerOfTwo(int n) {
            return n > 0 && (n & (n-1)) == 0;
        }
};
