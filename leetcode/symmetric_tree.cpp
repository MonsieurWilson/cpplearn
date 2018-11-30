/**
  * Copyright(C), 2018
  * Name: symmetric_tree
  * Author: Wilson Lan
  * Description:
  *     Given a binary tree, check whether it is a mirror of itself
  *     (ie, symmetric around its center).
  */

class Solution {
    public:
        /* Recursive solution */
        bool isSymmetric(TreeNode *root) {
            if (root == nullptr) {
                return true;
            }
            return isSymmetric(root->left, root->right);
        }

        bool isSymmetric(TreeNode *ltree, TreeNode *rtree) {
            if (ltree == nullptr && rtree == nullptr) {
                return true;
            } else if (ltree != nullptr && rtree != nullptr) {
                return (ltree->val == rtree->val) &&
                    isSymmetric(ltree->left,  rtree->right) &&
                    isSymmetric(ltree->right, rtree->left);
            } else {
                return false;
            }
        }

        /* Iterative solution with a queue */
        bool isSymmetricIterative(TreeNode *root) {
            queue<TreeNode *> unvisited;
            unvisited.push(root);
            unvisited.push(root);
            while (!unvisited.empty()) {
                TreeNode *ltree, *rtree;
                ltree = unvisited.front();
                unvisited.pop();
                rtree = unvisited.front();
                unvisited.pop();
                if (ltree == nullptr && rtree == nullptr) {
                    continue;
                } else if (ltree == nullptr || rtree == nullptr) {
                    return false;
                } else if (ltree->val != rtree->val) {
                    return false;
                }
                unvisited.push(ltree->left);
                unvisited.push(rtree->right);
                unvisited.push(rtree->left);
                unvisited.push(ltree->right);
            }
            return true;
        }
};
