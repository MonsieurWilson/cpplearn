/**
  * Copyright(C), 2018, Nsfocus
  * Name: postorder_traversal
  * Author: Wilson Lan
  * Description:
  *     Given a binary tree, return the postorder traversal 
  *     of its nodes' values.
  */

class Solution {
    public:
        vector<int> postorderTraversal(TreeNode *root) {
            vector<int> traversal;
            stack<TreeNode *> unvisited;
            TreeNode *ptr = root;
            while (ptr != nullptr || !unvisited.empty()) {
                while (ptr != nullptr) {
                    traversal.push_back(ptr->val);
                    unvisited.push(ptr);
                    ptr = ptr->right;
                }
                ptr = unvisited.top();
                unvisited.pop();
                ptr = ptr->left;
            }
            reverse(traversal.begin(), traversal.end());
            return traversal;
        }
};
