/*
 * @lc app=leetcode.cn id=283 lang=cpp
 *
 * [283] 移动零
 */

// @lc code=start
#include<vector>
using namespace std;

class Solution {
public:
    void moveZeroes(vector<int>& nums) {
        int slow = 0;
        for(int fast = 0; fast<nums.size()-1; fast++){
            if(nums[fast] !=0){
                nums[slow] = nums[fast];
                slow++;
            }
        }
        for(int i = slow; slow<nums.size()-1; i++){
            nums[i] = 0;
        }
    }
};
// @lc code=end

