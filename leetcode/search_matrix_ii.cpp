/**
  * Copyright(C), 2018
  * Name: search_matrix
  * Author: Wilson Lan
  * Description:
  *     Write an efficient algorithm that searches for a value in an 
  *     m x n matrix. This matrix has the following properties:
  *     - Integers in each row are sorted in ascending from left to right.
  *     - Integers in each column are sorted in ascending from top to bottom.
  */


class Solution {
public:
    /* Step-wise linear search algorithm of O(M+N) average-case time complexity */
    bool searchMatrix(vector<vector<int>> &matrix, int target) {
        bool found = false;
        auto nrows = matrix.size();
        if (nrows == 0) {
            return found;
        }
        auto ncols = matrix[0].size();
        if (ncols == 0) {
            return found;
        }

        int r = 0, c = ncols - 1;
        while (!found && r < nrows && c >= 0) {
            if (matrix[r][c] == target) {
                found = true;
                break;
            } else if (matrix[r][c] < target) {
                ++r;
            } else {
                --c;
            }
        }
        return found;
    }
};
