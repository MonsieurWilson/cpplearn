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
        vector<int> findErrorNums(vector<int>& nums) {
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
};
