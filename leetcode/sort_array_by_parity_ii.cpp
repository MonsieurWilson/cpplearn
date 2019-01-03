/**
  * Copyright(C), 2018
  * Name: sort_array_by_parity_ii
  * Author: Wilson Lan
  * Description:
  *     Given an array A of non-negative integers, half of the integers in A
  *     are odd, and half of the integers are even.
  *     Sort the array so that whenever A[i] is odd, i is odd; and whenever
  *     A[i] is even, i is even.
  *     You may return any answer array that satisfies this condition.
  */

class Solution {
    public:
        vector<int> sortArrayByParityII(vector<int> &A) {
            vector<int> res(A.size());
            int even = 0, odd = 1;
            for (const auto &elem : A) {
                if ((elem & 0x01) == 1) {
                    res[odd] = elem;
                    odd += 2;
                } else {
                    res[even] = elem;
                    even += 2;
                }
            }
            return res;
        }
};
