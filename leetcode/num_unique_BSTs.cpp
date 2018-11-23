/**
  * Copyright(C), 2018
  * Name: num_unique_BSTs
  * Author: Wilson Lan
  * Description:
  *     Given n, how many structurally unique BST's (binary search trees)
  *     that store values 1 ... n?
  */

class Solution {
    public:
        int numTrees(int n) {
            if (n == 0) {
                return 1;
            } else if (n <= 2) {
                return n;
            }

            int res = 0;
            for (int i = 1; i <= n; ++i) {
                res += numTrees(n - i) * numTrees(i - 1);
            }
            return res;
        }

        // DP solution
        int numTreesDP(int n) {
            vector<int> dp(n + 1);
            dp[0] = dp[1] = 1;
            numTreesDPImpl(n, dp);
            return dp[n];
        }

        int numTreesDPImpl(int n, vector<int> &dp) {
            if (dp[n] != 0) {
                return dp[n];
            }
            for (int i = 1; i <= n; ++i) {
                dp[n] += numTreesDPImpl(n - i, dp) * numTreesDPImpl(i - 1, dp);
            }
            return dp[n];
        }
};
