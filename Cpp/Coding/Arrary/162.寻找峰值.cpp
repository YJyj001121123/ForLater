/*
 * @lc app=leetcode.cn id=162 lang=cpp
 *
 * [162] 寻找峰值
 */

// @lc code=start
#include<vector>
#include<algorithm>
using namespace std;
class Solution {
public:
    int findPeakElement(vector<int>& nums) {
        return *max_element(nums.begin(),nums.end());
    }
    int findPeakElement2(vector<int>& nums) {
        int l = 0, r = nums.size()-1;
        while(l < r){
            int mid =  (r-l)/2 + l ;
            if(nums[mid] > nums[mid+1]) // 左边高，说明左边有峰值，可能mid就是
                r = mid;// mid在下一次查找中还要考虑在内
            else    l = mid+1;// 右边高，说明在mid右边有峰值，所以mid一定不是
        }
        return l;
    }
};
// @lc code=end

