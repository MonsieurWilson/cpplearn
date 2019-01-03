/**
  * Copyright(C), 2018
  * Name: monotonic_array
  * Author: Wilson Lan
  * Description:
  *     An array is monotonic if it is either monotone increasing or monotone
  *     decreasing.
  *     - An array A is monotone increasing if for all i <= j, A[i] <= A[j]. 
  *     - An array A is monotone decreasing if for all i <= j, A[i] >= A[j].
  *     Return true if and only if the given array A is monotonic.
  */

class Solution {
    public:
        bool isMonotonic(vector<int> &A) {
            int lens = A.size(), index = 1;
            int flag = 0;
            while (index < lens) {
                if (A[index] == A[index-1]) {
                } else if (flag == 0) {
                    flag = A[index] > A[index-1] ? 1 : -1;
                } else if (flag * (A[index] - A[index-1]) < 0) {
                    break;
                }
                ++index;
            }
            return lens == 0 || index == lens;
        }
};
