/**
  * Copyright(C), 2018
  * Name: BST_to_greater_tree
  * Author: Wilson Lan
  * Description:
  *     Given a Binary Search Tree (BST), convert it to a Greater Tree
  *     such that every key of the original BST is changed to the original
  *     key plus sum of all keys greater than the original key in BST.
  */

class Solution {
    private:
        int sum;

    public:
        /* Recursive solution */
        TreeNode *convertBST(TreeNode *root) {
            if (root != nullptr) {
                convertBST(root->right);
                sum += root->val;
                root->val = sum;
                convertBST(root->left);
            }
            return root;
        }

        /* Iterative solution with a stack */
        TreeNode *convertBSTIteration(TreeNode *root) {
            stack<TreeNode *> unvisited;
            TreeNode *ptr = root;
            while (ptr != nullptr || !unvisited.empty()) {
                while (ptr != nullptr) {
                    unvisited.push(ptr);
                    ptr = ptr->right;
                }
                ptr = unvisited.top();
                unvisited.pop();
                sum += ptr->val;
                ptr->val = sum;
                ptr = ptr->left;
            }
            return root;
        }
};
