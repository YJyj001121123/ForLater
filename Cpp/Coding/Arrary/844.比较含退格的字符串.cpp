/*
 * @lc app=leetcode.cn id=844 lang=cpp
 *
 * [844] 比较含退格的字符串
 */

// @lc code=start
#include<string>
using namespace std;

class Solution {
public:
    std::string backSpace(string s){
        int slow = 0;
        for(int f = 0; f<s.size()-1;f++){
            if(s[f] != '#'){
                s[slow++] = s[f];
            }else{
                if(slow>0){
                    slow--;
                }
            }
        }
        return s;
    }
    bool backspaceCompare(string s, string t) {
        auto s1 = backSpace(s);
        auto t1 = backSpace(t);
        if(s1 == t1) return true;
        return false;
    }
};
// @lc code=end

