
/***
 * 回归
 * 本质是穷举，穷举所有可能，选择答案
 * 回溯法一般是在集合中递归搜索，集合的大小构成了树的宽度，递归的深度构成的树的深度
 * 递归用来纵向遍历，for循环用来横向遍历
 * 
 * 组合问题：N个数里面按一定规则找出k个数的集合
 * 切割问题：一个字符串按一定规则有几种切割方式
 * 子集问题：一个N个数的集合里有多少符合条件的子集
 * 排列问题：N个数按一定规则全排列，有几种排列方式
 * 棋盘问题：N皇后，解数独等等 剔除掉 不看了
 * 
 * 组合问题和分割问题都是收集树的叶子节点，而子集问题是找树的所有节点
 * 
 * 怎么解决
 * 明确函数模板返回值参数 
 * 函数的终止条件
 * 回溯搜索的遍历过程
 * 
*/
// void backtracking(参数) {
//     if (终止条件) {
//         存放结果;
//         return;
//     }

//     for (选择：本层集合中元素（树中节点孩子的数量就是集合的大小）) {
//         处理节点;
//         backtracking(路径，选择列表); // 递归
//         回溯，撤销处理结果
//     }
// }
/***
 * 组合
 * 给定两个整数 n 和 k，返回 1 ... n 中所有可能的 k 个数的组合。
 * 输入: n = 4, k = 2 
 * 输出: [ [2,4], [3,4], [2,3], [1,2], [1,3], [1,4]]
 * 思路：抽象为树的结构  n相当于树的宽度，k相当于树的深度
*/
#include <vector>
#include <string>
#include <unordered_set>
std::vector<std::vector<int>> backTracking(int n, int k, int startIndex){
    //startIndex记录下层递归搜索的起始位置
    std::vector<std::vector<int>> result; // 存放符合条件结果的集合
    std::vector<int> path; // 用来存放符合条件单一结果
    if(path.size() == k){
        result.push_back(path);
        return result;
    }
    for(int i = startIndex; i <= n; i++){
        path.push_back(i);
        backTracking(n,k,i+1);
        path.pop_back(); //回溯 ，撤销处理的节点
    }
}
std::vector<std::vector<int>> combine(int n, int k){
    auto res = backTracking(n, k, 1);
    return res;
}

/***
 * 组合3
 * 找出所有相加之和为 n 的 k 个数的组合。组合中只允许含有 1 - 9 的正整数，并且每种组合中不存在重复的数字。
 * 输入: k = 3, n = 7
 * 输出: [[1,2,4]]
 * 思路：n相当于树的宽度，k相当于树的深度
*/
std::vector<std::vector<int>> backTracking2(int n, int k, int sum, int startIndex){
    std::vector<std::vector<int>> result; // 存放符合条件结果的集合
    std::vector<int> path; // 用来存放符合条件单一结果
    if(path.size() == k){
        if(sum = n) result.emplace_back(path);
        return result;
    }
    for(int i = startIndex; i<=9; i++){
        sum += i;
        path.emplace_back(i);
        backTracking2(n,k,sum,i+1);
        sum -= i;
        path.pop_back();
    }
}
std::vector<std::vector<int>> combine2(int k, int n){
    auto res = backTracking2(n,k,0,1);
    return res;
}

/***
 * 电话数字的字母组合
 * 给定一个仅包含数字 2-9 的字符串，返回所有它能表示的字母组合。给出数字到字母的映射如下（与电话按键相同）。注意 1 不对应任何字母。
 * 输入："23"
 * 输出：["ad", "ae", "af", "bd", "be", "bf", "cd", "ce", "cf"]
 * 思路：深度就是输入的长度
*/
const char* letterMap[10] = {
        "", // 0
        "", // 1
        "abc", // 2
        "def", // 3
        "ghi", // 4
        "jkl", // 5
        "mno", // 6
        "pqrs", // 7
        "tuv", // 8
        "wxyz", // 9
};
std::vector<std::string> backTracking3(std::string& digits, int index){
    //index是记录遍历第几个数字了，就是用来遍历digits的 树的深度。
    std::vector<std::string> result;
    std::string str;
    if(index == digits.size()){
        result.push_back(str);
        return result;
    }
    int digit = digits[index] - '0';
    std::string letters = letterMap[digit];
    for(int i = 0; i < letters.size(); i++){
        str.push_back(letters[i]);
        backTracking3(digits, index+1);
        str.pop_back();
    }
}
std::vector<std::string> letterCombinations(std::string digits) {
    if (digits.size() == 0) {
        return {};
    }
    auto res = backTracking3(digits, 0);
    return res;
}
/***
 * 组合总和
 * 给定一个无重复元素的数组 candidates 和一个目标数 target ，找出 candidates 中所有可以使数字和为 target 的组合。candidates 中的数字可以无限制重复被选取
 * 输入：candidates = [2,3,6,7], target = 7,
 *  [ [7], [2,2,3] ]
 * 思路：组合的情况下：是一个集合来求组合的话，就需要startIndex
 * 多个集合取组合，各个集合之间相互不影响，那么就不用startIndex
 * 
*/
std::vector<std::vector<int>> backTracking4(std::vector<int>& candidates, int target, int sum, int startIndex){
    std::vector<std::vector<int>> result;
    std::vector<int> path;
    if(sum>target) return result;
    if(sum == target) {
        result.push_back(path);
        return result;
    }
    for(int i = startIndex; i < candidates.size(); i++){
        sum += candidates[i];
        path.push_back(candidates[i]);
        backTracking4(candidates,target,sum,i);
        sum -= candidates[i];
        path.pop_back();
    }
}
std::vector<std::vector<int>> combinationSum(std::vector<int>& candidates, int target) {
    auto res = backTracking4(candidates, target, 0, 0);
    return res;
}
/***
 * 组合总和2
 * 给定一个数组 candidates 和一个目标数 target ，找出 candidates 中所有可以使数字和为 target 的组合。candidates 中的每个数字在每个组合中只能使用一次。
 * 输入: candidates = [10,1,2,7,6,1,5], target = 8,
 * [ [1, 7], [1, 2, 5], [2, 6], [1, 1, 6]]
 * 思路：需要去重复
*/
std::vector<std::vector<int>> backTracking5(std::vector<int>& candidates, int target, int sum, int startIndex, std::vector<bool>& used){
    std::vector<std::vector<int>> result;
    std::vector<int> path;
    if(sum == target) {
        result.push_back(path);
        return result;
    }
    for(int i = startIndex; i < candidates.size() && sum + candidates[i] <= target; i++){
         // used[i - 1] == true，说明同一树枝candidates[i - 1]使用过
        // used[i - 1] == false，说明同一树层candidates[i - 1]使用过
        // 要对同一树层使用过的元素进行跳过
        if (i > 0 && candidates[i] == candidates[i - 1] && used[i - 1] == false) {
            continue;
        }
        sum += candidates[i];
        path.push_back(candidates[i]);
        used[i] = true;
        backTracking5(candidates,target,sum,i+1,used);
        used[i] = false;
        sum -= candidates[i];
        path.pop_back();
    }
}
std::vector<std::vector<int>> combinationSum(std::vector<int>& candidates, int target) {
    std::vector<bool> used(candidates.size(), false);
    // 首先把给candidates排序，让其相同的元素都挨在一起。
    std::sort(candidates.begin(), candidates.end());
    auto res = backTracking5(candidates, target, 0, 0, used);
    return res;
}
/***
 * 分割回文串
 * 给定一个字符串 s，将 s 分割成一些子串，使每个子串都是回文串。返回 s 所有可能的分割方案。
 * 输入: "aab"
 * 输出: [ ["aa","b"], ["a","a","b"] ]
*/
bool isPalindrome(const std::string& s, int start, int end) {
    for (int i = start, j = end; i < j; i++, j--) {
        if (s[i] != s[j]) {
            return false;
        }
    }
    return true;
}
std::vector<std::vector<std::string>> backTracking6(std::string& s, int startIndex){
    std::vector<std::vector<std::string>> result;
    std::vector<std::string> path;
    // 如果起始位置已经大于s的大小，说明已经找到了一组分割方案了
    if (startIndex >= s.size()) {
        result.push_back(path);
        return result;
    }
    for(int i = startIndex; i < s.size() ; i++){
        if (isPalindrome(s, startIndex, i)) {   // 是回文子串
            // 获取[startIndex,i]在s中的子串
            std::string str = s.substr(startIndex, i - startIndex + 1);
            path.push_back(str);
        } else {                                // 不是回文，跳过
            continue;
        }
        backTracking6(s, i + 1); // 寻找i+1为起始位置的子串
        path.pop_back(); // 回溯过程，弹出本次已经添加的子串
    }
}
std::vector<std::vector<std::string>> partition(std::string s){
    auto res = backTracking6(s,0);
    return res;
}
/***
 * 复原IP地址
 * 给定一个只包含数字的字符串，复原它并返回所有可能的 IP 地址格式。
 * 有效的 IP 地址 正好由四个整数（每个整数位于 0 到 255 之间组成，且不能含有前导 0），整数之间用 '.' 分隔。
 * 输入：s = "25525511135"  s = "1111"
 * 输出：["255.255.11.135","255.255.111.35"]  ["1.1.1.1"]
 * 思路：切割问题
*/
class solution{
    std::vector<std::string> result;
    // 判断字符串s在左闭又闭区间[start, end]所组成的数字是否合法
    bool isValid(const std::string& s, int start, int end) {
        if (start > end) {
            return false;
        }
        if (s[start] == '0' && start != end) { // 0开头的数字不合法
                return false;
        }
        int num = 0;
        for (int i = start; i <= end; i++) {
            if (s[i] > '9' || s[i] < '0') { // 遇到非数字字符不合法
                return false;
            }
            num = num * 10 + (s[i] - '0');
            if (num > 255) { // 如果大于255了不合法
                return false;
            }
        }
        return true;
    }
    // startIndex: 搜索的起始位置，pointNum:添加逗点的数量
    void backTracking(std::string& s, int startIndex, int pointNum){
        if (pointNum == 3) { // 逗点数量为3时，分隔结束
            // 判断第四段子字符串是否合法，如果合法就放进result中
            if (isValid(s, startIndex, s.size() - 1)) {
                result.push_back(s);
            }
            return;
        }
        for (int i = startIndex; i < s.size(); i++) {
            if (isValid(s, startIndex, i)) { // 判断 [startIndex,i] 这个区间的子串是否合法
                s.insert(s.begin() + i + 1 , '.');  // 在i的后面插入一个逗点
                pointNum++;
                backTracking(s, i + 2, pointNum);   // 插入逗点之后下一个子串的起始位置为i+2
                pointNum--;                         // 回溯
                s.erase(s.begin() + i + 1);         // 回溯删掉逗点
            } else break; // 不合法，直接结束本层循环
        }
    }
public:
    std::vector<std::string> restoreIpAddresses(std::string s) {
        result.clear();
        if (s.size() < 4 || s.size() > 12) return result; // 算是剪枝了
        backTracking(s, 0, 0);
        return result;
    }
};
/***
 * 子集
 * 给定一组不含重复元素的整数数组 nums，返回该数组所有可能的子集（幂集）
 * 输入: nums = [1,2,3]
 * 输出: [ [3],   [1],   [2],   [1,2,3],   [1,3],   [2,3],   [1,2],   [] ]
*/
std::vector<std::vector<int>> backTracking7(std::vector<int>& nums, int startIndex){
    std::vector<std::vector<int>> result;
    std::vector<int> path;
    result.push_back(path); // 收集子集，要放在终止添加的上面，否则会漏掉自己
    if (startIndex >= nums.size()) { // 终止条件可以不加
        return result;
    }
    for (int i = startIndex; i < nums.size(); i++) {
        path.push_back(nums[i]);
        backTracking7(nums, i + 1);
        path.pop_back();
    }
}
std::vector<std::vector<int>> subsets(std::vector<int>& nums) {
        auto res = backTracking7(nums, 0);
        return res;
}
/***
 * 子集II
 * 给定一个可能包含重复元素的整数数组 nums，返回该数组所有可能的子集（幂集）。
 * 输入: [1,2,2]
 * 输出: [ [2], [1], [1,2,2], [2,2], [1,2], [] ]
*/
class Solution2 {
private:
    std::vector<std::vector<int>> result;
    std::vector<int> path;
    void backtracking(std::vector<int>& nums, int startIndex) {
        result.push_back(path);
        std::unordered_set<int> uset;
        for (int i = startIndex; i < nums.size(); i++) {
            if (uset.find(nums[i]) != uset.end()) {
                continue;
            }
            uset.insert(nums[i]);
            path.push_back(nums[i]);
            backtracking(nums, i + 1);
            path.pop_back();
        }
    }

public:
    std::vector<std::vector<int>> subsetsWithDup(std::vector<int>& nums) {
        result.clear();
        path.clear();
        std::sort(nums.begin(), nums.end()); // 去重需要排序
        backtracking(nums, 0);
        return result;
    }
};
/***
 * 递增子序列
 * 给定一个整型数组, 你的任务是找到所有该数组的递增子序列，递增子序列的长度至少是2。
 * 输入: [4, 6, 7, 7]
 * 输出: [[4, 6], [4, 7], [4, 6, 7], [4, 6, 7, 7], [6, 7], [6, 7, 7], [7,7], [4,7,7]]
 * 思路：取有序的子集，去重复
*/
// 版本一
class Solution3 {
private:
    std::vector<std::vector<int>> result;
    std::vector<int> path;
    void backtracking(std::vector<int>& nums, int startIndex) {
        if (path.size() > 1) {
            result.push_back(path);
            // 注意这里不要加return，要取树上的节点
        }
        std::unordered_set<int> uset; // 使用set对本层元素进行去重
        for (int i = startIndex; i < nums.size(); i++) {
            if ((!path.empty() && nums[i] < path.back())
                    || uset.find(nums[i]) != uset.end()) {
                    continue;
            }
            uset.insert(nums[i]); // 记录这个元素在本层用过了，本层后面不能再用了
            path.push_back(nums[i]);
            backtracking(nums, i + 1);
            path.pop_back();
        }
    }
public:
    std::vector<std::vector<int>> findSubsequences(std::vector<int>& nums) {
        result.clear();
        path.clear();
        backtracking(nums, 0);
        return result;
    }
};
/***
 * 全排列
 * 给定一个 没有重复 数字的序列，返回其所有可能的全排列。
 * 输入: [1,2,3]
 * 输出: [ [1,2,3], [1,3,2], [2,1,3], [2,3,1], [3,1,2], [3,2,1] ]
*/
class Solution4 {
public:
    std::vector<std::vector<int>> result;
    std::vector<int> path;
    void backtracking (std::vector<int>& nums, std::vector<bool>& used) {
        // 此时说明找到了一组
        if (path.size() == nums.size()) {
            result.push_back(path);
            return;
        }
        for (int i = 0; i < nums.size(); i++) {
            if (used[i] == true) continue; // path里已经收录的元素，直接跳过
            used[i] = true;
            path.push_back(nums[i]);
            backtracking(nums, used);
            path.pop_back();
            used[i] = false;
        }
    }
    std::vector<std::vector<int>> permute(std::vector<int>& nums) {
        std::vector<bool> used(nums.size(), false);
        backtracking(nums, used);
        return result;
    }
};
/***
 * 全排列 II
 * 给定一个可包含重复数字的序列 nums ，按任意顺序 返回所有不重复的全排列。
 * 输入：nums = [1,1,2]
 * 输出： [[1,1,2], [1,2,1], [2,1,1]]
*/
class Solution5 {
private:
    std::vector<std::vector<int>> result;
    std::vector<int> path;
    void backtracking (std::vector<int>& nums, std::vector<bool>& used) {
        // 此时说明找到了一组
        if (path.size() == nums.size()) {
            result.push_back(path);
            return;
        }
        for (int i = 0; i < nums.size(); i++) {
            // used[i - 1] == true，说明同一树枝nums[i - 1]使用过
            // used[i - 1] == false，说明同一树层nums[i - 1]使用过
            // 如果同一树层nums[i - 1]使用过则直接跳过
            if (i > 0 && nums[i] == nums[i - 1] && used[i - 1] == false) {
                continue;
            }
            if (used[i] == false) {
                used[i] = true;
                path.push_back(nums[i]);
                backtracking(nums, used);
                path.pop_back();
                used[i] = false;
            }
        }
    }
public:
    std::vector<std::vector<int>> permuteUnique(std::vector<int>& nums) {
        result.clear();
        path.clear();
        std::sort(nums.begin(), nums.end()); // 排序
        std::vector<bool> used(nums.size(), false);
        backtracking(nums, used);
        return result;
    }
};