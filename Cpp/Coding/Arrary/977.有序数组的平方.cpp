/*
 * @lc app=leetcode.cn id=977 lang=cpp
 *
 * [977] 有序数组的平方
 */

// @lc code=start
#include<vector>
using namespace std;

class Solution {
public:
    // vector<int> sortedSquares(vector<int>& nums) {
    //     std::vector<int>res(nums.size());
    //     int l = 0;
    //     int r = nums.size()-1;
    //     int index = r;
    //     while(l<=r){
    //         if(nums[l]*nums[l]>nums[r]*nums[r]){
    //             res[index--]=nums[l] * nums[l] ;
    //             l++;
    //         }else {
    //             res[index--] = nums[r] * nums[r];
    //             r--;
    //         }
    //     }
    //     return res;
    // }
    vector<int> sortedSquares(vector<int>& nums) {
        for(int i = 0; i < nums.size(); i++){
            nums[i] *= nums[i]; 
        }
        std::sort(nums.begin(),nums.end());
        return nums;
    }
};
// @lc code=end

