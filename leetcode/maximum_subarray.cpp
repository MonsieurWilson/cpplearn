/**
  * Copyright(C), 2018, Nsfocus
  * Name: maximum_subarray
  * Author: Wilson Lan
  * Description:
  *     Given an integer array nums, find the contiguous subarray 
  *     (containing at least one number) which has the largest 
  *     sum and return its sum.
  */

#include <vector>
#include <iostream>
#include <climits>

using std::vector;
using std::cout;
using std::endl;

class Solution {
    public:
        int maxSubArray(vector<int> &nums) {
            if (nums.size() == 0) {
                return 0;
            }

            // `max_tail` represents maximum subarray with tail element.
            int max_tail, maximum;
            max_tail = maximum = INT_MIN;
            for (auto &num : nums) {
                max_tail = max_tail > 0 ? max_tail+num : num;
                maximum  = std::max(maximum, max_tail);
            }

            return maximum;
        }

        int maxSubArray(vector<int> &nums,
                        vector<int>::size_type &left,
                        vector<int>::size_type &right) {
            if (nums.size() == 0) {
                return 0;
            }

            vector<int>::size_type left_max_tail = 0;
            int max_tail, maximum;
            max_tail = maximum = nums[0];
            left = right = 0;
            for (vector<int>::size_type i = 1; i < nums.size(); ++i) {
                if (max_tail < 0) {
                    max_tail = 0;
                    left_max_tail = i;
                }
                max_tail += nums[i];
                if (maximum < max_tail) {
                    left = left_max_tail;
                    right = i;
                    maximum = max_tail;
                }
            }

            return maximum;
        }
};

int main() {
    vector<int> nums1{-2,1,-3,4,-1,2,1,-5,4};
    vector<int> nums2{-2,-2,-1,-1};
    vector<int> nums3{1,2};
    vector<int> nums4{8,-19,5,-4,20};
    vector<int> nums5{-2147483647};
    Solution s;

    cout << s.maxSubArray(nums1) << endl;
    cout << s.maxSubArray(nums2) << endl;
    cout << s.maxSubArray(nums3) << endl;
    cout << s.maxSubArray(nums4) << endl;
    cout << s.maxSubArray(nums5) << endl;
}
