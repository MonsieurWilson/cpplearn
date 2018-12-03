/**
  * Copyright(C), 2018
  * Name: narytree_postorder_traversal
  * Author: Wilson Lan
  * Description:
  *     Given an n-ary tree, return the postorder traversal of its nodes'
  *     values.
  */

class Node {
    public:
        int val;
        vector<Node *> children;
        
        Node() {}
        Node(int _val, vector<Node *> _children)
            : val(_val), children(_children) {}
};

class Solution {
    public:
        vector<int> postorder(Node *root) {
            vector<int> traversal;
            postorderImpl(root, traversal);
            return traversal;
        }

        void postorderImpl(Node *root, vector<int> &traversal) {
            if (root == nullptr) {
                return;
            }
            for (const auto &c : root->children) {
                postorderImpl(c, traversal);
            }
            traversal.push_back(root->val);
        }
};
