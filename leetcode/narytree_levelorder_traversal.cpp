/**
  * Copyright(C), 2018
  * Name:
  * Author: Wilson Lan
  * Description:
  *     Given an n-ary tree, return the level order traversal
  *     of its nodes' values. (ie, from left to right, level by level).
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
        vector<vector<int>> levelOrder(Node *root) {
            vector<vector<int>> traversal;
            queue<Node *> unvisited;
            if (root == nullptr) {
                return traversal;
            }
            unvisited.push(root);
            while (!unvisited.empty()) {
                int lens = unvisited.size();
                vector<int> levelTraversals;
                for (int cnt = 1; cnt <= lens; ++cnt) {
                    Node *ptr = unvisited.front();
                    levelTraversals.push_back(ptr->val);
                    unvisited.pop();
                    for (Node *c : ptr->children) {
                        unvisited.push(c);
                    }
                }
                traversal.push_back(levelTraversals);
            }
            return traversal;
        }
};
