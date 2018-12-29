/**
  * Copyright(C), 2018
  * Name: n_repeated_elements_in_2n_array
  * Author: Wilson Lan
  * Description:
  *     In a array A of size 2N, there are N+1 unique elements, and exactly one
  *     of these elements is repeated N times.
  *     Return the element repeated N times.
  *
  *     Example 1:
  *     Input: [1,2,3,3]
  *     Output: 3
  *
  *     Example 2:
  *     Input: [2,1,2,5,3,2]
  *     Output: 2
  *
  *     Example 3:
  *     Input: [5,1,5,2,5,3,5,4]
  *     Output: 5
  */

class Solution {
    public:
        int repeatedNTimes(vector<int> &A) {
            int ret = A[0];
            // Note the case [2,1,3,2]
            for (int i = 0; i < A.size()-2; ++i) {
                if (A[i] == A[i+1] || A[i] == A[i+2]) {
                    ret = A[i];
                    break;
                }
            }
            return ret;
        }

        /* A randomized solution of O(1) average-case time complexity. */
        int repeatedNTimes(vector<int> &A) {
            int i = 0, j = 0, N = A.size();
            while (i == j || A[i] != A[j]) {
                i = rand() % N;
                j = rand() % N;
            }
            return A[i];
        }
};
