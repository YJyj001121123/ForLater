/*
 * @lc app=leetcode.cn id=54 lang=cpp
 *
 * [54] 螺旋矩阵
 */

// @lc code=start
#include <vector>
using namespace std;

class Solution {
public:
    vector<int> spiralOrder(vector<vector<int>>& matrix) {
        std::vector<int> res;
        int m = matrix.size();
        int n = matrix[0].size();
        int top = 0;
        int bottom = m-1;
        int left = 0;
        int right = n-1;
        while(true){
            for(int lie = left; lie <= right; ++lie){
                res.emplace_back(matrix[top][lie]);
            }
            ++top;
            if(top>bottom) break;

            for(int hang = top; hang<= bottom; ++hang){
                res.emplace_back(matrix[hang][right]);
            }
            --right;
            if(right<left) break;

            for(int lie = right; lie>= right; --lie){
                res.emplace_back(matrix[bottom][lie]);
            }
            --bottom;
            if(top>bottom) break;

            for(int hang = bottom; hang>=top; --hang){
                res.emplace_back(matrix[hang][left]);
            }
            ++left;
            if(right<left) break;
        }
        return  res;
    }
};
// @lc code=end

