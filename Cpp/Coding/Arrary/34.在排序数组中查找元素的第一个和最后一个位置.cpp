/*
 * @lc app=leetcode.cn id=34 lang=cpp
 *
 * [34] 在排序数组中查找元素的第一个和最后一个位置
 */

// @lc code=start
#include <vector>
using namespace std;
class Solution {
private:
    int getRightOrder(vector<int>& nums, int target){
        int left = 0;
        int right = nums.size()-1;
        int rightOrder = -2;
        while(left<=right){
            int mid = (right-left)/2 + left;
            if(nums[mid]>target){
                right = mid-1;
            }else{
                left = mid+1;
                rightOrder = left;
            }
        }
        return rightOrder;
    }
    int getLeftOrder(vector<int>& nums, int target){
        int left = 0;
        int right = nums.size()-1;
        int leftOrder = -2;
        while(left<=right){
            int mid = (right-left)/2 + left;
            if(nums[mid]>=target){
                right = mid-1;
                leftOrder = right;
            }else{
                left = mid+1;
            }
        }
        return leftOrder;
    }
public:
    vector<int> searchRange(vector<int>& nums, int target) {
        int left_res = getLeftOrder(nums,target);
        int right_res = getRightOrder(nums,target);
        if(left_res == -2 || right_res == -2) return {-1,-1};
        if(right_res - left_res > 1) return{left_res+1, right_res-1};
        return {-1,-1};
    }
};
// @lc code=end

