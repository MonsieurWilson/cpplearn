/**
  * Copyright(C), 2018
  * Name: zigzag_levelorder_traversal
  * Author: Wilson Lan
  * Description:
  *     Given a binary tree, return the zigzag level order traversal of 
  *     its nodes' values. (ie, from left to right, then right to left 
  *     for the next level and alternate between).
  */

class Solution {
    public:
        vector<vector<int>> zigzagLevelOrder(TreeNode *root) {
            vector<vector<int>> traversal;
            queue<TreeNode *> unvisited;
            int flag = 1;
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
                    if (ptr->left != nullptr) {
                        unvisited.push(ptr->left);
                    }
                    if (ptr->right != nullptr) {
                        unvisited.push(ptr->right);
                    }
                    unvisited.pop();
                }
                if (flag < 0) {
                    reverse(levelTraversals.begin(), levelTraversals.end());
                }
                flag *= -1;
                traversal.push_back(levelTraversals);
            }
            return traversal;
        }
};
