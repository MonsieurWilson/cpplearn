/**
  * Copyright(C), 2018
  * Name: balanced_binarytree
  * Author: Wilson Lan
  * Description:
  *     Given a binary tree, determine if it is height-balanced.
  *     For this problem, a height-balanced binary tree is defined as:
  *
  *     Note: a binary tree in which the depth of the two subtrees of every
  *     node never differ by more than 1.
  */

class Solution {
    public:
        bool isBalanced(TreeNode *root) {
            int depth;
            return isBalancedImpl(root, depth);
        }

        bool isBalancedImpl(TreeNode *root, int &depth) {
            if (root == nullptr) {
                depth = 0;
                return true;
            }
            int ldepth, rdepth;
            bool ltreeIsBalanced = isBalancedImpl(root->left, ldepth),
                 rtreeIsBalanced = isBalancedImpl(root->right, rdepth);
            depth = max(ldepth, rdepth) + 1;
            return ltreeIsBalanced && rtreeIsBalanced &&
                abs(ldepth-rdepth) <= 1;
        }
};
