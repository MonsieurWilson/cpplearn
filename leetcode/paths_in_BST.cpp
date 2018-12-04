/**
  * Copyright(C), 2018
  * Name: paths_in_BST
  * Author: Wilson Lan
  * Description:
  *     Given a binary tree, return all root-to-leaf paths.
  *
  *     Note: A leaf is a node with no children.
  */

class Solution {
    public:
        vector<string> binaryTreePaths(TreeNode *root) {
            if (root == nullptr) {
                return {};
            } else if (root->left == nullptr && root->right == nullptr) {
                return {to_string(root->val)};
            }
            vector<string> paths;
            auto lpath = binaryTreePaths(root->left),
                 rpath = binaryTreePaths(root->right);
            paths.insert(paths.end(), lpath.begin(), lpath.end());
            paths.insert(paths.end(), rpath.begin(), rpath.end());

            /*
             * `transform` equals to the following code block.
             *  for (auto &p : paths) {
             *      p.insert(0, to_string(root->val)+"->");
             *  }
             */
            transform(make_move_iterator(paths.begin()),
                 make_move_iterator(paths.end()),
                 paths.begin(),
                 [root](auto &&p) { return to_string(root->val) + "->" + p; });
            return paths;
        }
};
