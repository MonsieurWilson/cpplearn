/**
  * Copyright(C), 2018
  * Name: flatten_BST_inorder
  * Author: Wilson Lan
  * Description:
  *     Given a tree, rearrange the tree in in-order so that the leftmost node
  *     in the tree is now the root of the tree, and every node has no left
  *     child and only 1 right child.
  */

class Solution {
    public:
        TreeNode *increasingBST(TreeNode *root) {
            if (root == nullptr) {
                return nullptr;
            }
            return flatten(root).first;
        }

        pair<TreeNode *, TreeNode *> flatten(TreeNode *root) {
            TreeNode *head = root, *tail = root;
            if (root->right != nullptr) {
                auto endpoint = flatten(root->right);
                tail = endpoint.second;
                root->right = endpoint.first;
            }
            if (root->left != nullptr) {
                auto endpoint = flatten(root->left);
                endpoint.second->right = root;
                head = endpoint.first;
                root->left = nullptr;
            }
            return make_pair(head, tail);
        }
};
