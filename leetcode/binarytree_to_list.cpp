/**
  * Copyright(C), 2018
  * Name: binarytree_to_list
  * Author: Wilson Lan
  * Description:
  *     Given a binary tree, flatten it to a linked list in-place.
  */

class Solution {
    public:
        void flatten(TreeNode *root) {
            if (root != nullptr) {
                flattenImpl(root);
            }
        }

        pair<TreeNode *, TreeNode *> flattenImpl(TreeNode *root) {
            TreeNode *tail = root;
            if (root->right != nullptr) {
                tail = flattenImpl(root->right).second;
            }
            if (root->left != nullptr) {
                auto endpoint = flattenImpl(root->left);
                root->left = nullptr;
                if (root->right != nullptr) {
                    endpoint.second->right = root->right;
                } else {
                    tail = endpoint.second;
                }
                root->right = endpoint.first;
            }
            return make_pair(root, tail);
        }
};
