/**
  * Copyright(C), 2018
  * Name: set_mismatch
  * Author: Wilson Lan
  * Description:
  *     The set S originally contains numbers from 1 to n. But unfortunately,
  *     due to the data error, one of the numbers in the set got duplicated to
  *     another number in the set, which results in repetition of one number
  *     and loss of another number.
  *
  *     Given an array nums representing the data status of this set after the
  *     error. Your task is to firstly find the number occurs twice and then 
  *     find the number that is missing. Return them in the form of an array.
  */

class Solution {
    public:
        vector<int> findErrorNums(vector<int> &nums) {
            int dupelem, misselem;
            dupelem = misselem = 1;
            for (int n : nums) {
                if (nums[abs(n)-1] < 0) {
                    dupelem = abs(n);
                } else {
                    nums[abs(n)-1] *= -1;
                }
            }
            for (int i = 0; i < nums.size(); ++i) {
                if (nums[i] > 0) {
                    misselem = i + 1;
                    break;
                }
            }
            return {dupelem, misselem};
        }

        vector<int> findErrorNumsXOR(vector<int> &nums) {
            // Calculate the xor of duplicate and missing elements.
            int dupxormiss = 0;
            for (int n : nums) {
                dupxormiss ^= n;
            }
            for (int i = 1; i <= nums.size(); ++i) {
                dupxormiss ^= i;
            }

            // Seperate into two parts and calculate xor1, xor2.
            // xor1 and xor2 must be duplicate and missing elements.
            int rightmostbit = dupxormiss & ~(dupxormiss - 1);
            int xor1 = 0, xor2 = 0;
            for (int n : nums) {
                if (n & rightmostbit) {
                    xor1 ^= n;
                } else {
                    xor2 ^= n;
                }
            }
            for (int i = 1; i <= nums.size(); ++i) {
                if (i & rightmostbit) {
                    xor1 ^= i;
                } else {
                    xor2 ^= i;
                }
            }

            // Find which one is the element of set.
            auto pos = find(nums.begin(), nums.end(), xor1);
            if (pos == nums.end()) {
                return {xor2, xor1};
            } else {
                return {xor1, xor2};
            }
        }
};
