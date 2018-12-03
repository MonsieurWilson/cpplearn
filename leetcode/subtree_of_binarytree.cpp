/**
  * Copyright(C), 2018
  * Name: subtree_of_binarytree
  * Author: Wilson Lan
  * Description:
  *     Given two non-empty binary trees s and t, check whether tree t has
  *     exactly the same structure and node values with a subtree of s. A
  *     subtree of s is a tree consists of a node in s and all of this node's
  *     descendants. The tree s could also be considered as a subtree of itself.
  */

class Solution {
    public:
        bool isSubtree(TreeNode *s, TreeNode *t) {
            return isEqualTree(s, t) || s != nullptr &&
                (isSubtree(s->left, t) || isSubtree(s->right, t));
        }
        
        bool isEqualTree(TreeNode *s, TreeNode *t) {
            if (s == nullptr && t == nullptr) {
                return true;
            } else if (s == nullptr || t == nullptr) {
                return false;
            }
            return s->val == t->val &&
                isEqualTree(s->left, t->left) &&
                isEqualTree(s->right, t->right);
        }
};
