/**
  * Copyright(C), 2018
  * Name: permutations_ii
  * Author: Wilson Lan
  * Description:
  *     Given a collection of numbers that might contain duplicates,
  *     return all possible unique permutations.
  */

class Solution {
    public:
        vector<vector<int>> permuteUnique(vector<int> &nums) {
            vector<vector<int>> permutations;
            permuteUniqueImpl(nums, 0, permutations);
            return permutations;
        }

        void permuteUniqueImpl(vector<int> &nums, int index,
                               vector<vector<int>> &permutations) {
            int lens = nums.size();
            if (index >= lens - 1) {
                permutations.push_back(nums);
                return;
            }
            unordered_set<int> s;
            for (int i = index; i < lens; ++i) {
                if (s.find(nums[i]) != s.end()) {
                    continue;
                }
                s.insert(nums[i]);
                swap(nums[i], nums[index]);
                permuteUniqueImpl(nums, index+1, permutations);
                swap(nums[i], nums[index]);
            }
        }
};
