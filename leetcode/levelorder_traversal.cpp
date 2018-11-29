/**
  * Copyright(C), 2018
  * Name: levelorder_traversal
  * Author: Wilson Lan
  * Description:
  *     Given a binary tree, return the level order traversal of its nodes'
  *     values. (ie, from left to right, level by level).
  */

class Solution {
    public:
        vector<vector<int>> levelOrder(TreeNode *root) {
            vector<vector<int>> traversal;
            queue<TreeNode *> unvisited;
            if (root == nullptr) {
                return traversal;
            }
            unvisited.push(root);
            while (!unvisited.empty()) {
                int lens = unvisited.size();
                vector<int> levelTraversals;
                for (int cnt = 1; cnt <= lens; ++cnt) {
                    TreeNode *ptr = unvisited.front();
                    levelTraversals.push_back(ptr->val);
                    unvisited.pop();
                    if (ptr->left != nullptr) {
                        unvisited.push(ptr->left);
                    }
                    if (ptr->right != nullptr) {
                        unvisited.push(ptr->right);
                    }
                }
                traversal.push_back(levelTraversals);
            }
            return traversal;
        }
};
