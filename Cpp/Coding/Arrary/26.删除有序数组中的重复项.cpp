/*
 * @lc app=leetcode.cn id=26 lang=cpp
 *
 * [26] 删除有序数组中的重复项
 */

// @lc code=start
#include<vector>
using namespace std;
class Solution {
public:
    int removeDuplicates(vector<int>& nums) {
        int slowIndex = 0;
        for(int fastIndex = 1; fastIndex < nums.size(); fastIndex++){
            if(nums[fastIndex]!=nums[fastIndex-1]){
                slowIndex++;
                nums[slowIndex] = nums[fastIndex];
            }
        }
        return slowIndex+1;
    }
};
// @lc code=end

