/**
  * Copyright(C), 2018
  * Name: sort_array_by_parity
  * Author: Wilson Lan
  * Description:
  *     Given an array A of non-negative integers, return an array consisting
  *     of all the even elements of A, followed by all the odd elements of A.
  *     You may return any answer array that satisfies this condition.
  */

class Solution {
    public:
        vector<int> sortArrayByParity(vector<int> &A) {
            vector<int> res(A);
            int head = 0, tail = res.size() - 1;
            while (head < tail) {
                while (head < tail && 0 == (res[head] & 0x01)) ++head;
                while (head < tail && 1 == (res[tail] & 0x01)) --tail;
                if (head < tail) {
                    swap(res[head++], res[tail--]);
                }
            }
            return res;
        }

        /* Using partition function of C++ standard library */
        vector<int> sortArrayByParity(vector<int> &A) {
            vector<int> res(A);
            partition(res.begin(), res.end(), [] (const int &num) {
                return (num & 0x01) == 0;
            });
            return res;
        }
};
