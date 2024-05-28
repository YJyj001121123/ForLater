/*
 * @lc app=leetcode.cn id=69 lang=cpp
 *
 * [69] x 的平方根 
 */

// @lc code=start
class Solution {
public:
    int mySqrt(int x) {
        int left = 0;
        int right = x;
        while(left<=right){
             int mid = (right - left) / 2 + left;
            if (x / mid >= mid) left = mid + 1;
            else right = mid;
        }
        return right-1;
    }
};
// @lc code=end

