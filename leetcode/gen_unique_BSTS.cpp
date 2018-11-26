/**
  * Copyright(C), 2018
  * Name: gen_unique_BSTs
  * Author: Wilson Lan
  * Description:
  *     Given an integer n, generate all structurally unique BST's
  *     (binary search trees) that store values 1 ... n.
  */

class Solution {
    public:
        vector<TreeNode *> generateTrees(int n) {
            if (n < 1) {
                return {};
            }
            return generateTreesImpl(1, n);
        }

        vector<TreeNode *> generateTreesImpl(int begin, int end) {
            if (begin > end) {
                return {nullptr};
            }
            vector<TreeNode *> trees, ltrees, rtrees;
            for (int i = begin; i <= end; ++i) {
                ltrees = generateTreesImpl(begin, i - 1);
                rtrees = generateTreesImpl(i + 1, end);
                for (auto l : ltrees) {
                    for (auto r : rtrees) {
                        TreeNode *root = new TreeNode(i);
                        root->left = l;
                        root->right = r;
                        trees.push_back(root);
                    }
                }
            }
            return trees;
        }
};
