/**
  * Copyright(C), 2018
  * Name: invert_tree
  * Author: Wilson Lan
  * Description:
  *     Invert a binary tree.
  */

class Solution {
    public:
        TreeNode *invertTree(TreeNode *root) {
            if (root == nullptr) {
                return nullptr;
            }
            std::swap(root->left, root->right);
            root->left  = invertTree(root->left);
            root->right = invertTree(root->right);
            return root;
        }
};
