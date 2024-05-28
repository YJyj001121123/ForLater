/*
 * @lc app=leetcode.cn id=367 lang=cpp
 *
 * [367] 有效的完全平方数
 */

// @lc code=start
class Solution {
public:
    bool isPerfectSquare(int num) {
        int left = 0;
        int right = num;
        while(left<=right){
            int mid = (right-left)/2 + left;
            long sqr = (long)mid*mid;
            if(sqr>num){
                right = mid-1;
            }else if(sqr<num){
                left = mid+1;
            }else{
                return true;
            }
        }
        return false;
    }
};
// @lc code=end

