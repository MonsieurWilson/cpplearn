/**
  * Copyright(C), 2018
  * Name: my_pow
  * Author: Wilson Lan
  * Description:
  *     Implement pow(x, n), which calculates x raised to the power n (x^n).
  */

class Solution {
public:
    double myPow(double x, int n) {
        if (equal(x, 0.0) || equal(x, 1.0)) {
            return x;
        }
        if (n == 0) {
            return 1;
        }

        double res = myPow(x, n / 2);
        res *= res;
        if (n & 0x1) {
            n > 0 ? res *= x : res /= x;
        }
        return res;
    }

    bool equal(double x, double y) {
        double sub = x - y > 0 ? x - y : y - x;
        return sub < std::numeric_limits<double>::epsilon();
    }
};
