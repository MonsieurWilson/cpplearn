/**
  * Copyright(C), 2018
  * Name: average_of_levels
  * Author: Wilson Lan
  * Description:
  *     Given a non-empty binary tree, return the average value of 
  *     the nodes on each level in the form of an array.
  */

class TreeNode {
    public:
        int val;
        TreeNode *left;
        TreeNode *right;

        TreeNode(int x) 
            : val(x), left(nullptr), right(nullptr) {}
};

class Solution {
    public:
        vector<double> averageOfLevels(TreeNode *root) {
            vector<double> res;
            queue<TreeNode *> unvisited;
            if (root == nullptr) {
                return res;
            }
            unvisited.push(root);
            while (!unvisited.empty()) {
                int lens = unvisited.size();
                double avg = 0;
                for (int cnt = 1; cnt <= lens; ++cnt) {
                    TreeNode *ptr = unvisited.front();
                    avg += ptr->val;
                    unvisited.pop();
                    if (ptr->left != nullptr) {
                        unvisited.push(ptr->left);
                    }
                    if (ptr->right != nullptr) {
                        unvisited.push(ptr->right);
                    }
                }
                res.push_back(avg / lens);
            }
            return res;
        }
};
