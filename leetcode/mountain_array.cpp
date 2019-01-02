/**
  * Copyright(C), 2018
  * Name: mountain_array
  * Author: Wilson Lan
  * Description:
  *     Given an array A of integers, return true if and only if it is a valid
  *     mountain array.
  *     Recall that A is a mountain array if and only if:
  *     - A.length >= 3
  *     - There exists some i with 0 < i < A.length - 1 such that:
  *       - A[0] < A[1] < ... A[i-1] < A[i]
  *       - A[i] > A[i+1] > ... > A[B.length - 1]
  */

class Solution {
    public:
        bool validMountainArray(vector<int> &A) {
            int N = A.size(), low = 0, high = N - 1;
            while (low < high && A[low]  < A[low+1])  ++low;
            while (low < high && A[high] < A[high-1]) --high;
            return low == high && low != 0 && low != N - 1;
        }
};
