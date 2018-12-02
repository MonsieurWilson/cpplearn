/**
  * Copyright(C), 2018
  * Name: traversal_to_binarytree
  * Author: Wilson Lan
  * Description:
  *     Given inorder and postorder traversal of a tree, construct the
  *     binary tree.
  *
  *     Note:
  *     You may assume that duplicates do not exist in the tree.
  */

class Solution {
    public:
        TreeNode *buildTree(vector<int> &inorder, vector<int> &postorder) {
            return buildTreeImpl(inorder.begin(), inorder.end(),
                                 postorder.begin(), postorder.end());
        }

        TreeNode *buildTreeImpl(vector<int>::const_iterator inorder_first,
                                vector<int>::const_iterator inorder_last,
                                vector<int>::const_iterator postorder_first,
                                vector<int>::const_iterator postorder_last) {
            if (inorder_first >= inorder_last) {
                return nullptr;
            }
            int root_val = *(postorder_last - 1);
            TreeNode *root = new TreeNode(root_val);
            auto root_pos = std::find(inorder_first, inorder_last, root_val);
            auto n_ltree_nodes = root_pos - inorder_first;
            root->left  = buildTreeImpl(inorder_first, root_pos,
                                        postorder_first, postorder_first
                                        +n_ltree_nodes);
            root->right = buildTreeImpl(root_pos+1, inorder_last,
                                        postorder_first+n_ltree_nodes,
                                        postorder_last-1);
            return root;
        }
};
