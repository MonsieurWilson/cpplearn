/**
  * Copyright(C), 2018
  * Name: find_duplicates
  * Author: Wilson Lan
  * Description:
  *     Given an array of integers, 1 ≤ a[i] ≤ n (n = size of array),
  *     some elements appear twice and others appear once.
  *
  *     Find all the elements that appear twice in this array.
  */

class Solution {
    public:
        vector<int> findDuplicates(vector<int> &nums) {
            vector<int> dups;
            for (int n : nums) {
                if (nums[abs(n)-1] < 0) {
                    dups.push_back(abs(n));
                } else {
                    nums[abs(n)-1] *= -1;
                }
            }
            return dups;
        }
};
