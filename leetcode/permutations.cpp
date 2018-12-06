/**
  * Copyright(C), 2018
  * Name: permutations
  * Author: Wilson Lan
  * Description:
  *     Given a collection of distinct integers, return all possible
  *     permutations.
  */

class Solution {
    public:
        vector<vector<int>> permute(vector<int> &nums) {
            vector<vector<int>> permutations;
            permuteImpl(nums, 0, permutations);
            return permutations;
        }

        void permuteImpl(vector<int> &nums, int index,
                         vector<vector<int>> &permutations) {
            int lens = nums.size();
            if (lens == 0) {
                return;
            }
            permutations.push_back(nums);
            for (int i = index; i < lens; ++i) {
                for (int j = i + 1; j < lens; ++j) {
                    swap(nums[i], nums[j]);
                    permuteImpl(nums, i+1, permutations);
                    swap(nums[i], nums[j]);
                }
            }
        }
};
