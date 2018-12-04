/**
  * Copyright(C), 2018
  * Name: print_binarytree
  * Author: Wilson Lan
  * Description:
  * 	Example 1:
  *         Input: 1
  *             /
  *            2
  *         Output:
  *         [["", "1", ""],
  *          ["2", "", ""]]
  *     Example 2:
  *         Input:
  *              1
  *             / \
  *            2   3
  *             \
  *              4
  *         Output:
  *         [["", "", "", "1", "", "", ""],
  *          ["", "2", "", "", "", "3", ""],
  *          ["", "", "4", "", "", "", ""]]
  */

class Solution {
    public:
        vector<vector<string>> printTree(TreeNode *root) {
            int depth = maxDepth(root);
            vector<string> line((1<<depth)-1, "");
            vector<vector<string>> output(depth, line);
            printTreeImpl(root, 0, 0, line.size()-1, output);
            return output;
        }

        void printTreeImpl(TreeNode *root, int depth, int left, int right,
                           vector<vector<string>> &output) {
            if (root == nullptr) {
                return;
            }
            output[depth][(left+right)/2] = to_string(root->val);
            printTreeImpl(root->left,  depth+1, left, (left+right)/2, output);
            printTreeImpl(root->right, depth+1, (left+right)/2+1, right, output);
        }

        int maxDepth(TreeNode *root) {
            if (root == nullptr) {
                return 0;
            }
            return 1 + max(maxDepth(root->left), maxDepth(root->right));
        }
};
