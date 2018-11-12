/**
  * Copyright(C), 2018
  * Name: search_matrix
  * Author: Wilson Lan
  * Description:
  *     Write an efficient algorithm that searches for a value in an 
  *     m x n matrix. This matrix has the following properties:
  *     - Integers in each row are sorted from left to right.
  *     - The first integer of each row is greater than the last 
  *       integer of the previous row.
  */


class Solution {
public:
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

        int begin = 0, end = nrows * ncols - 1;
        int mid, pivot;
        while (!found && begin <= end) {
            mid = (begin + end) / 2;
            pivot = matrix[mid/ncols][mid%ncols];
            if (pivot == target) {
                found = true;
                break;
            } else if (pivot < target) {
                begin = pivot + 1;
            } else {
                end = pivot - 1;
            }
        }
        return found;
    }
};
