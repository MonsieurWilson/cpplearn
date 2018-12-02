/**
  * Copyright(C), 2018
  * Name: same_tree
  * Author: Wilson Lan
  * Description:
  *     Given two binary trees, write a function to check if they are the
  *     same or not.
  *     Two binary trees are considered the same if they are structurally
  *     identical and the nodes have the same value.
  */

class Solution {
    public:
        bool isSameTree(TreeNode *p, TreeNode *q) {
            if (p == nullptr && q == nullptr) {
                return true;
            } else if (p != nullptr && q != nullptr) {
                return p->val == q->val &&
                    isSameTree(p->left, q->left) &&
                    isSameTree(p->right, q->right);
            } else {
                return false;
            }
        }
};
