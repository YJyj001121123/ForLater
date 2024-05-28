/*
 * @lc app=leetcode.cn id=35 lang=cpp
 *
 * [35] 搜索插入位置
 */

// @lc code=start
#include<vector>
using namespace std;
class Solution {
public:
    int searchInsert(vector<int>& nums, int target) {
        int left = 0;
        int right = nums.size()-1;
        while(left<=right){
            int mid = (right-left)/2 + left;
            if(nums[mid]>target){
                right = mid-1;
            }else if(nums[mid]<target){
                left = mid +1;
            }else{
                return mid;
            }
        }
        return right+1;
    }
};
// @lc code=end

