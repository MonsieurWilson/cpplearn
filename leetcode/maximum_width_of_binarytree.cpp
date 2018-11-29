/**
  * Copyright(C), 2018
  * Name: maximum_width_of_binarytree
  * Author: Wilson Lan
  * Description:
  *     Given a binary tree, write a function to get the maximum width of
  *     the given tree. The width of a tree is the maximum width among all
  *     levels. The binary tree has the same structure as a full binary tree,
  *     but some nodes are null.
  *
  *     The width of one level is defined as the length between the end-nodes
  *     (the leftmost and right most non-null nodes in the level, where the
  *     null nodes between the end-nodes are also counted into the length
  *     calculation.
  */

class Solution {
    public:
        int widthOfBinaryTree(TreeNode *root) {
            int width = 0;
            deque<TreeNode *> unvisited;
            if (root == nullptr) {
                return width;
            }
            unvisited.push_back(root);
            while (!unvisited.empty()) {
                int lens = unvisited.size();
                width = std::max(width, lens);
                for (int cnt = 1; cnt <= lens; ++cnt) {
                    TreeNode *ptr = unvisited.front();
                    unvisited.push_back(ptr ? ptr->left  : ptr);
                    unvisited.push_back(ptr ? ptr->right : ptr);
                    unvisited.pop_front();
                }
                // Trim heading and trailing null pointers.
                while (!unvisited.empty() && !unvisited.front()) {
                    unvisited.pop_front();
                }
                while (!unvisited.empty() && !unvisited.back()) {
                    unvisited.pop_back();
                }
            }
            return width;
        }

        /* Keep track of the index of each TreeNode instead of pushing
         * all nodes of a level and operate trimming. */
        int widthOfBinaryTreeStoreIndex(TreeNode *root) {
            int width = 0;
            // Pair structure: <node pointer, index>
            queue<pair<TreeNode *, int>> unvisited;
            if (root == nullptr) {
                return width;
            }
            unvisited.push(make_pair(root, 0));
            while (!unvisited.empty()) {
                int lmostpos = INT_MAX, rmostpos = INT_MIN;
                int lens = unvisited.size();
                for (int cnt = 1; cnt <= lens; ++cnt) {
                    auto elem = unvisited.front();
                    unvisited.pop();
                    if (elem.first->left != nullptr) {
                        unvisited.push(make_pair(elem.first->left, 
                                                 elem.second*2));
                    }
                    if (elem.first->right != nullptr) {
                        unvisited.push(make_pair(elem.first->right,
                                                 elem.second*2+1));
                    }
                    lmostpos = std::min(lmostpos, elem.second);
                    rmostpos = std::max(rmostpos, elem.second);
                }
                width = std::max(width, rmostpos-lmostpos+1);
            }
            return width;
        }
};
