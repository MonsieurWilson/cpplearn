/**
  * Copyright(C), 2018
  * Name: narytree_preorder_traversal
  * Author: Wilson Lan
  * Description:
  *     Given an n-ary tree, return the preorder traversal of its nodes' values.
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
        vector<int> preorder(Node *root) {
            vector<int> traversal;
            preorderImpl(root, traversal);
            return traversal;
        }

        void preorderImpl(Node *root, vector<int> &traversal) {
            if (root == nullptr) {
                return;
            }
            traversal.push_back(root->val);
            for (const auto &c : root->children) {
                preorderImpl(c, traversal);
            }
        }
};
