/**
  * Copyright(C), 2018, Nsfocus
  * Name: preorder_traversal
  * Author: Wilson Lan
  * Description:
  *     Given a binary tree, return the preorder traversal 
  *     of its nodes' values.
  */

class Solution {
    public:
        vector<int> preorderTraversal(TreeNode *root) {
            vector<int> traversal;
            stack<TreeNode *> unvisited;
            TreeNode *ptr = root;
            while (ptr != nullptr || !unvisited.empty()) {
                while (ptr != nullptr) {
                    traversal.push_back(ptr->val);
                    unvisited.push(ptr);
                    ptr = ptr->left;
                }
                ptr = unvisited.top();
                unvisited.pop();
                ptr = ptr->right;
            }
            return traversal;
        }
};
