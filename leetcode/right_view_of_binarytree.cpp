/**
  * Copyright(C), 2018
  * Name: right_view_of_binarytree
  * Author: Wilson Lan
  * Description:
  *     Given a binary tree, imagine yourself standing on the right side
  *     of it, return the values of the nodes you can see ordered from
  *     top to bottom.
  */

class Solution {
    public:
        vector<int> rightSideView(TreeNode *root) {
            vector<int> view;
            int depth = 0;
            rightSideViewImpl(root, 1, depth, view);
            return view;
        }

        void rightSideViewImpl(TreeNode *root, int curDepth, int &depth,
                               vector<int> &view) {
            if (root == nullptr) {
                return;
            }
            if (curDepth > depth) {
                depth = curDepth;
                view.push_back(root->val);
            }
            rightSideViewImpl(root->right, curDepth+1, depth, view);
            rightSideViewImpl(root->left,  curDepth+1, depth, view);
        }
};
