/**
  * Copyright(C), 2018
  * Name: tile_of_binarytree
  * Author: Wilson Lan
  * Description:
  *     Given a binary tree, return the tilt of the whole tree.
  *     The tilt of a tree node is defined as the absolute difference between
  *     the sum of all left subtree node values and the sum of all right
  *     subtree node values. Null node has tilt 0.
  *     The tilt of the whole tree is defined as the sum of all nodes' tilt.
  */

class Solution {
    public:
        int findTilt(TreeNode *root) {
            int sum = 0;
            return findTiltImpl(root, sum);
        }

        int findTiltImpl(TreeNode *root, int &sum) {
            if (root == nullptr) {
                sum = 0;
                return 0;
            }
            int lsum = 0, rsum = 0;
            int ltilt = findTiltImpl(root->left, lsum),
                rtilt = findTiltImpl(root->right, rsum);
            sum = lsum + rsum + root->val;
            return abs(lsum - rsum) + ltilt + rtilt;
        }
};
