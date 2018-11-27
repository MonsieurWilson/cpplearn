/**
  * Copyright(C), 2018
  * Name: reverse_string
  * Author: Wilson Lan
  * Description:
  *     Write a function that takes a string as input and returns
  *     the string reversed.
  */

class Solution {
    public:
        string reverseString(string s) {
            auto lens = s.size();
            for (string::size_type i = 0; i < lens / 2; ++i) {
                std::swap(s[i], s[lens-1-i]);
            }
            return s;
        }
};
