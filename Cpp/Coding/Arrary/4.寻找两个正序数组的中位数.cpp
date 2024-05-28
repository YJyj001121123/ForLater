/*
 * @lc app=leetcode.cn id=4 lang=cpp
 *
 * [4] 寻找两个正序数组的中位数
 */

// @lc code=start
#include <vector>
using namespace std;
class Solution {
private:
    std::vector<int> mergeNums(vector<int>& nums1, vector<int>& nums2){
        std::vector<int> res;
        int i = 0, j = 0;
        while (i < nums1.size() && j < nums2.size()) {
            if (nums1[i] < nums2[j]) {
                res.push_back(nums1[i]);
                i++;
            } else {
                res.push_back(nums2[j]);
                j++;
            }
        }
        // 将剩余的元素加入结果向量
        while (i < nums1.size()) {
            res.push_back(nums1[i]);
            i++;
        }
        while (j < nums2.size()) {
            res.push_back(nums2[j]);
            j++;
        }
        return res;
    }
public:
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        auto res = mergeNums(nums1,nums2);
        int n = res.size();
        if (n % 2 == 1) { // 如果数组长度为奇数
            return res[n / 2];
        } else { // 如果数组长度为偶数
            int mid1 = res[n / 2 - 1];
            int mid2 = res[n / 2];
            return (mid1 + mid2) / 2.0;
        }
    }
};
// @lc code=end

