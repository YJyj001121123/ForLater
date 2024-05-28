/*
 * @lc app=leetcode.cn id=76 lang=cpp
 *
 * [76] 最小覆盖子串
 */

// @lc code=start
#include<string>
using namespace std;
class Solution {
public:
    string minWindow(string s, string t) {
        unordered_map<char, int> hash4t, hash4s;
        int left = 0;
        int count = 0;
        string res;
        for(int i = 0; i < t.size(); ++i){//统计t的字符出现次数
            hash4t[t[i]]++;
        }
        //遍历字符串s
        for(int right = 0; right < s.size(); ++right){
            hash4s[s[right]]++;//标记出现过的元素
            if(hash4s[s[right]] <= hash4t[s[right]]){//如果当前字符为有效目标字符，计数加1
                count++;
            }
            //处理窗口左边界
            while(hash4s[s[left]] > hash4t[s[left]]){
                hash4s[s[left]]--;//对应计数值减减
                left++;//窗口左边界右移
            }
            //处理收集到3个有效目标字符时的情况
            if(count == t.size()){
                if(res.empty() || right - left + 1 < res.size()){//保存最小的子串
                    res = s.substr(left, right - left + 1);
                }
            }
        }
        return res;
    }
};

// @lc code=end

