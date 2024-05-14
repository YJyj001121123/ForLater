#include <string>
#include <unordered_set>
/***
 * hash table
 * 数组就是一张哈希表
 * 一般哈希表都是用来快速判断一个元素是否出现集合里。
 * 
 * 哈希碰撞
 * 拉链法--冲突元素放入list
 * 探测--空位补充
*/
//有效字母异位词
//输入: s = "anagram", t = "nagaram" 
//输出: true
//思路：定义一个数组叫做record用来上记录字符串s里字符出现的次数
bool isAnagram(std::string s, std::string t){
    std::vector<int> record(26,0);
    for(int i = 0; i<s.size();i++){
        record[s[i]-'a']++;
    }
    for (int i = 0; i < t.size(); i++) {
            record[t[i] - 'a']--;
    }
    for (int i = 0; i < 26; i++) {
        if (record[i] != 0) {
            // record数组如果有的元素不为零0，说明字符串s和t 一定是谁多了字符或者谁少了字符。
            return false;
        }
    }
    return true;
}
//查找常用字符
//输入：words = ["bella","label","roller"] 
//输出：["e","l","l"]
std::vector<char> commonChar(std::vector<std::string>& words){
    std::vector<char> res;
    if(words.size()==0) return res;
    std::vector<int> hash(26, 0); // 用来统计所有字符串里字符出现的最小频率
    for(auto c : words[0]) hash[c-'a']++;
    for (int i = 1; i < words.size(); ++i) {
        std::vector<int> hashOtherStr(26, 0); // 统计除第一个字符串外字符的出现频率
        for (const auto& ch : words[i]) hashOtherStr[ch - 'a']++;
        // 更新 hash，保证 hash 里统计26个字符在所有字符串里出现的最小次数
        for (int k = 0; k < 26; ++k) hash[k] = std::min(hash[k], hashOtherStr[k]);
    }
    // 将 hash 统计的字符次数，转成输出形式
    for (int i = 0; i < 26; ++i) {
        if (hash[i] != 0) {
            std::string s(1, i + 'a'); // char -> string
            res.emplace(res.end(), hash[i], s);
        }
    }
    return res;
}
//两个数组交集
//输入：[1,2,2,1] [2,2]
//输出：[2]
std::vector<int> intersection(std::vector<int>& nums1, std::vector<int>& nums2) {
    std::unordered_set<int> result_set; // 存放结果，之所以用set是为了给结果集去重
    std::unordered_set<int> nums_set(nums1.begin(), nums1.end());
    for (int num : nums2) {
        // 发现nums2的元素 在nums_set里又出现过
        if (nums_set.find(num) != nums_set.end()) {
            result_set.insert(num);
        }
    }
    return std::vector<int>(result_set.begin(), result_set.end());
}
//快乐数
//对于一个正整数，每一次将该数替换为它每个位置上的数字的平方和，
//然后重复这个过程直到这个数变为 1，也可能是 无限循环 但始终变不到 1。
//如果 可以变为  1，那么这个数就是快乐数
// 输入：19
// 输出：true
// 解释：
// 1^2 + 9^2 = 82
// 8^2 + 2^2 = 68
// 6^2 + 8^2 = 100
// 1^2 + 0^2 + 0^2 = 1
int getSum(int n){
    int sum =0;
    while(n){
        sum += (n%10)*(n%10);
        n /= 10;
    }
    return sum;
}
bool isHappy(int n) {
    std::unordered_set<int> set;
    while(1) {
        int sum = getSum(n);
        if (sum == 1) {
            return true;
        }
        // 如果这个sum曾经出现过，说明已经陷入了无限循环了，立刻return false
        if (set.find(sum) != set.end()) {
            return false;
        } else {
            set.insert(sum);
        }
        n = sum;
    }
}
//两数之和
std::vector<int> twoSum3(std::vector<int>& nums, int k){
    std::unordered_map<int,int> hash;
    if(nums.size()==0) return {};
    for(int i = 0; i<nums.size(); ++i){
       auto it = hash.find(k-nums[i]);
       if(it != hash.end()) return {it->second,i};
       hash[nums[i]] = i; //没有找到 放入hash表
    }
    return {};
}
//四数相加--计算有多少个元组 (i, j, k, l) ，使得 A[i] + B[j] + C[k] + D[l] = 0
// 输入:
// A = [ 1, 2]
// B = [-2,-1]
// C = [-1, 2]
// D = [ 0, 2]
// 输出: 2
//解释:两个元组如下:
// (0, 0, 0, 1) -> A[0] + B[0] + C[0] + D[1] = 1 + (-2) + (-1) + 2 = 0
// (1, 1, 0, 0) -> A[1] + B[1] + C[0] + D[0] = 2 + (-1) + (-1) + 0 = 0
int fourSumCount(std::vector<int>& A, std::vector<int>& B, 
                std::vector<int>& C, std::vector<int>& D) {
    std::unordered_map<int, int> umap; //key:a+b的数值，value:a+b数值出现的次数
    // 遍历大A和大B数组，统计两个数组元素之和，和出现的次数，放到map中
    for (int a : A) {
        for (int b : B) {
            umap[a + b]++;
        }
    }
    int count = 0; // 统计a+b+c+d = 0 出现的次数
    // 在遍历大C和大D数组，找到如果 0-(c+d) 在map中出现过的话，就把map中key对应的value也就是出现次数统计出来。
    for (int c : C) {
        for (int d : D) {
            if (umap.find(0 - (c + d)) != umap.end()) {
                count += umap[0 - (c + d)];
            }
        }
    }
    return count;
}
//三数之和为0
//输入：[-1, 0, 1, 2, -1, -4] k = 0
//输出：[-1, 0, 1], [-1, -1, 2]
//思路：排序+双指针
std::vector<std::vector<int>> threeSum(std::vector<int>& nums){
    std::vector<std::vector<int>> res;
    if(nums.size()<3) return res;
    std::sort(nums.begin(),nums.end());
    for(int i=0; i < nums.size(); i++){
        if(i!=0 && nums[i]==nums[i-1]) continue;//如果元素相同，那么后面元素的情况已经被包含
        int left=i+1, right=nums.size()-1;
        while(left<right){
            int sum=nums[left]+nums[right]+nums[i];
            if(0==sum){
                std::vector<int> temp;
                temp.push_back(nums[i]);
                temp.push_back(nums[left++]);
                temp.push_back(nums[right--]);
                res.push_back(temp);
                //下面两行代码用于去掉重复。一定要有left<right,否则出错。如[0,0,0]
                while(left<right && nums[left]==nums[left-1]) ++left;
                while(left<right && nums[right]==nums[right+1]) --right;
            }
            else if(sum<0) left++;
            else  right--;
        }
    }
    return res;
}
//四数之和--判断 nums 中是否存在四个元素 a，b，c 和 d ，使得 a + b + c + d 的值与 target 相等
//输入：nums = [1, 0, -1, 0, -2, 2]，和 target = 0
//输出： [ [-1, 0, 0, 1], [-2, -1, 1, 2], [-2, 0, 0, 2] ]
std::vector<std::vector<int>> fourSum(std::vector<int>& nums,int target){
    std::vector<std::vector<int>> res;
    if(nums.size()<4) return res;
    std::sort(nums.begin(),nums.end());
    for(int k = 0; k < nums.size(); k++){
        if(nums[k] > target && nums[k]>=0) break;
        if(k>0 && nums[k] == nums[k-1]) continue;
        for(int i = k+1; i<nums.size();i++){
            if(nums[k]+nums[i]>target&&nums[k]+nums[i]>=0) break;
            if (i > k + 1 && nums[i] == nums[i - 1]) continue;
            int left = i+1;
            int right = nums.size()-1;
            while(right>left){
                if ((long) nums[k] + nums[i] + nums[left] + nums[right] > target) {
                    right--;
                // nums[k] + nums[i] + nums[left] + nums[right] < target 会溢出
                } else if ((long) nums[k] + nums[i] + nums[left] + nums[right]  < target) {
                    left++;
                } else {
                    res.push_back(std::vector<int>{nums[k], nums[i], nums[left], nums[right]});
                    // 对nums[left]和nums[right]去重
                    while (right > left && nums[right] == nums[right - 1]) right--;
                    while (right > left && nums[left] == nums[left + 1]) left++;

                    // 找到答案时，双指针同时收缩
                    right--;
                    left++;
                }
            }
        }
    } 
    return res;
}
//同构字符串
//输入：s = "egg", t = "add"
//输出：true
//输入：s = "foo", t = "bar"
//输出：false
bool isIsomorphic(std::string s, std::string t) {
    std::unordered_map<char, char> map1;
    std::unordered_map<char, char> map2;
    for (int i = 0, j = 0; i < s.size(); i++, j++) {
        if (map1.find(s[i]) == map1.end()) { // map1保存s[i] 到 t[j]的映射
            map1[s[i]] = t[j];
        }
        if (map2.find(t[j]) == map2.end()) { // map2保存t[j] 到 s[i]的映射
            map2[t[j]] = s[i];
        }
        // 发现映射 对应不上，立刻返回false
        if (map1[s[i]] != t[j] || map2[t[j]] != s[i]) {
            return false;
        }
    }
    return true;
}