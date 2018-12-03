/**
  * Copyright(C), 2018
  * Name: depth_of_binarytree
  * Author: Wilson Lan
  * Description:
  *     Given a binary tree, find its maximum depth.
  *     The maximum depth is the number of nodes along the longest path
  *     from the root node down to the farthest leaf node.
  *     Note: A leaf is a node with no children.
  */

class Solution {
    public:
        /* Recursive solution */
        int maxDepth(TreeNode *root) {
            if (root == nullptr) {
                return 0;
            }
            return 1 + max(maxDepth(root->left), maxDepth(root->right));
        }

        /* Iterative solution */
        int maxDepthIterative(TreeNode *root) {
            if (root == nullptr) {
                return 0;
            }
            int depth = 0;
            stack<pair<TreeNode *, int>> unvisited;
            unvisited.push(make_pair(root, 1));
            while (!unvisited.empty()) {
                auto elem = unvisited.top();
                unvisited.pop();
                TreeNode *ptr = elem.first;
                int cur_depth = elem.second;
                depth = max(depth, cur_depth);
                if (ptr->left != nullptr) {
                    unvisited.push(make_pair(ptr->left, cur_depth+1));
                }
                if (ptr->right != nullptr) {
                    unvisited.push(make_pair(ptr->right, cur_depth+1));
                }
            }
            return depth;
        }
};
