/**
  * Copyright(C), 2018
  * Name: diameter_of_binarytree
  * Author: Wilson Lan
  * Description:
  *     Given a binary tree, you need to compute the length of the diameter
  *     of the tree. The diameter of a binary tree is the length of the
  *     longest path between any two nodes in a tree. This path may or may
  *     not pass through the root.
  */

class Solution {
    public:
        int diameterOfBinaryTree(TreeNode *root) {
            int depth;
            return diameterOfBinaryTreeImpl(root, depth);
        }

        int diameterOfBinaryTreeImpl(TreeNode *root, int &depth) {
            if (root == nullptr) {
                depth = 0;
                return 0;
            } else if (!root->left && !root->right) {
                depth = 1;
                return 0;
            } else {
                int ldepth, rdepth;
                int ldiameter = diameterOfBinaryTreeImpl(root->left,  ldepth),
                    rdiameter = diameterOfBinaryTreeImpl(root->right, rdepth);
                depth = max(ldepth, rdepth) + 1;
                return max(max(ldiameter, rdiameter), rdepth+ldepth);
            }
        }
};
