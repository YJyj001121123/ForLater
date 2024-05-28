/*
 * @lc app=leetcode.cn id=904 lang=cpp
 *
 * [904] 水果成篮
 */

// @lc code=start
#include<vector>
using namespace std;

class Solution {
public:
    int totalFruit(vector<int>& fruits) {
        int left = 0;//定义左指针
        int res = 0;
        unordered_map<int, int> typeCount;//定义一个哈希表，键是遍历值
        for(int right = 0; right < fruits.size(); ++right){
            typeCount[fruits[right]]++;//记录出现次数
            while(typeCount.size() > 2){//如果类型大于2，开始移动左指针缩小窗口
                auto it = typeCount.find(fruits[left]);//使用find找到键值对，并返回迭代体对象
                it->second--;//将键对应的值全部删除
                if(it->second == 0){
                    typeCount.erase(it); //删除指向键值对的迭代体
                }
                left++;//左指针右移
            }
            res = max(res, right - left + 1);//将最大值更新到结果变量
        }
        return res;
    }
};

// @lc code=end

