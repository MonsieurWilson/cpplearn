/**
  * Copyright(C), 2018, Nsfocus
  * Name: inorder_traversal
  * Author: Wilson Lan
  * Description:
  *     Given a binary tree, return the inorder traversal 
  *     of its nodes' values.
  */

class Solution {
    public:
        vector<int> inorderTraversal(TreeNode *root) {
            vector<int> traversal;
            stack<TreeNode *> unvisited;
            TreeNode *ptr = root;
            while (ptr != nullptr || !unvisited.empty()) {
                while (ptr != nullptr) {
                    unvisited.push(ptr);
                    ptr = ptr->left;
                }
                ptr = unvisited.top();
                unvisited.pop();
                traversal.push_back(ptr->val);
                ptr = ptr->right;
            }
            return traversal;
        }
};
