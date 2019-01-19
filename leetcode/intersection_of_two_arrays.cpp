/**
  * Copyright(C), 2019
  * Name: intersection_of_two_arrays
  * Author: Wilson Lan
  * Description:
  *     key plus sum of all keys greater than the original key in BST.
  * Note:
  *     - Each element in the result must be unique.
  *     - The result can be in any order.
  */

class Solution {
public:
    vector<int> intersection(vector<int> &nums1, vector<int> &nums2) {
        unordered_set<int> s1(nums1.begin(), nums1.end()),
                           s2(nums2.begin(), nums2.end());
        vector<int> res;
        for (const auto &num : s1) {
            if (s2.find(num) != s2.end()) {
                res.push_back(num);
            }
        }
        return res;
    }
};
