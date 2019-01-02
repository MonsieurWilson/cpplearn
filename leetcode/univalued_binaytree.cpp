/**
  * Copyright(C), 2018
  * Name: univalued_binaytree
  * Author: Wilson Lan
  * Description:
  *     A binary tree is univalued if every node in the tree has the same value.
  *     Return true if and only if the given tree is univalued.
  */

class Solution {
    public:
        bool isUnivalTree(TreeNode *root) {
            if (root == nullptr) {
                return true;
            }
            bool res = true;
            res &= root->left == nullptr ? 
                true : root->val == root->left->val;
            res &= root->right == nullptr ? 
                true : root->val == root->right->val;
            return res && isUnivalTree(root->left) && 
                isUnivalTree(root->right);
        }
};
