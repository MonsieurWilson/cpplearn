/**
  * Copyright(C), 2018
  * Name: maximum_binarytree
  * Author: Wilson Lan
  * Description:
  *     Given an integer array with no duplicates. A maximum tree
  *     building on this array is defined as follow:
  *     - The root is the maximum number in the array.
  *     - The left subtree is the maximum tree constructed from left
  *       part subarray divided by the maximum number.
  *     - The right subtree is the maximum tree constructed from right
  *       part subarray divided by the maximum number.
  *
  *     Construct the maximum tree by the given array and output the root
  *     node of this tree.
  *
  *     Note that this kind of tree is also called Cartesian tree.
  */

class Solution {
    public:
        /* Recursive solution of O(NlogN) average-case time complexity
         * and O(N^2) worst-case complexity. The average-case space 
         * complexity is O(logN), and worst-case space complexity is O(N).
         */
        TreeNode *constructMaximumBinaryTree(vector<int> &nums) {
            return constructImpl(nums.begin(), nums.end());
        }

        TreeNode *constructImpl(vector<int>::const_iterator first,
                                vector<int>::const_iterator last) {
            if (first == last) {
                return nullptr;
            }
            auto maxitr = std::max_element(first, last);
            TreeNode *root = new TreeNode(*maxitr);
            root->left  = constructImpl(first, maxitr);
            root->right = constructImpl(maxitr+1, last);
            return root;
        }

        /* Iterative solution of O(N) time complexity. The worst-case
         * space complexity is O(N). */
        TreeNode *constructMaximumBinaryTreeIterative(vector<int> &nums) {
            vector<TreeNode *> nodes;
            for (const auto v : nums) {
                TreeNode *cur  = new TreeNode(v);
                while (!nodes.empty() && nodes.back()->val < v) {
                    cur->left = nodes.back();
                    nodes.pop_back();
                }
                if (!nodes.empty()) {
                    nodes.back()->right = cur;
                }
                nodes.push_back(cur);
            }
            return nodes.front();
        }
};
