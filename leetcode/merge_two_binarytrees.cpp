/**
  * Copyright(C), 2018
  * Name: merge_two_binarytrees
  * Author: Wilson Lan
  * Description:
  *     Given two binary trees and imagine that when you put one of them
  *     to cover the other, some nodes of the two trees are overlapped
  *     while the others are not.
  *
  *     You need to merge them into a new binary tree. The merge rule is
  *     that if two nodes overlap, then sum node values up as the new value
  *     of the merged node. Otherwise, the NOT null node will be used as the
  *     node of new tree.
  */

class Solution {
    public:
        TreeNode *mergeTrees(TreeNode *t1, TreeNode *t2) {
            TreeNode *root = nullptr;
            if (t1 == nullptr && t2 == nullptr) {
            } else if (t1 != nullptr && t2 != nullptr) {
                root = new TreeNode(t1->val+t2->val);
                root->left = mergeTrees(t1->left, t2->left);
                root->right = mergeTrees(t1->right, t2->right);
            } else {
                TreeNode *tmp = t1 ? t1 : t2;
                root = new TreeNode(tmp->val);
                root->left = mergeTrees(tmp->left, nullptr);
                root->right = mergeTrees(tmp->right, nullptr);
            }
            return root;
        }
};
