/**
  * Copyright(C), 2018
  * Name: find_mode_in_BST
  * Author: Wilson Lan
  * Description:
  *     Given a binary search tree (BST) with duplicates, find all the mode(s)
  *     (the most frequently occurred element) in the given BST.
  *
  *     Assume a BST is defined as follows:
  *     - The left subtree of a node contains only nodes with keys
  *       less than or equal to the node's key.
  *     - The right subtree of a node contains only nodes with keys
  *       greater than or equal to the node's key.
  *     - Both the left and right subtrees must also be binary search trees.
  */

class Solution {
    private:
        vector<int> modes;
        TreeNode *pre;
        int maximum, cnt;

    public:
        vector<int> findMode(TreeNode *root) {
            cnt = 1;
            findModeImpl(root);
            return modes;
        }

        void findModeImpl(TreeNode *root) {
            if (root == nullptr) {
                return;
            }
            findModeImpl(root->left);
            if (pre != nullptr) {
                root->val == pre->val ? ++cnt : cnt = 1;
            }
            if (cnt >= maximum) {
                if (cnt > maximum) {
                    modes.clear();
                }
                modes.push_back(root->val);
                maximum = cnt;
            }
            pre = root;
            findModeImpl(root->right);
        }
};
