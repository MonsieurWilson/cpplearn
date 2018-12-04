/**
  * Copyright(C), 2018
  * Name: search_in_BST
  * Author: Wilson Lan
  * Description:
  *     Given the root node of a binary search tree (BST) and a value.
  *     You need to find the node in the BST that the node's value equals
  *     the given value. Return the subtree rooted with that node. If such
  *     node doesn't exist, you should return NULL.
  */

class Solution {
    public:
        TreeNode *searchBST(TreeNode *root, int val) {
            TreeNode *cur = root;
            while (cur != nullptr) {
                if (cur->val == val) {
                    break;
                } else if (cur->val > val) {
                    cur = cur->left;
                } else {
                    cur = cur->right;
                }
            }
            return cur;
        }
};
