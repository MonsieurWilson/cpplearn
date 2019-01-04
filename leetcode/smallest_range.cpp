/**
  * Copyright(C), 2018
  * Name: smallest_range
  * Author: Wilson Lan
  * Description:
  *     Given an array A of integers, for each integer A[i] we may choose any
  *     x with -K <= x <= K, and add x to A[i].
  *     After this process, we have some array B.
  *     Return the smallest possible difference between the maximum value of B
  *     and the minimum value of B.
  */

class Solution {
    public:
        int smallestRange(vector<int> &A, int K) {
            int minElem = INT_MAX, maxElem = INT_MIN;
            for (const auto &e : A) {
                minElem = min(e, minElem);
                maxElem = max(e, maxElem);
            }
            return max(maxElem-minElem-2*K, 0);
        }
};
