#include <string>

/***
 * 字符串
 * 在C语言中，把一个字符串存入一个数组时，也把结束符 '\0'存入数组，并以此作为该字符串是否结束的标志
 * 在C++中，提供一个string类，string类会提供 size接口，可以用来判断string类字符串是否结束，就不用'\0'来判断是否结束
 * 
*/
//反转字符串
//输入：["h","e","l","l","o"]
//输出：["o","l","l","e","h"]
//思路：双指针
void reverseString(std::vector<char>& s){
    for(int i = 0; j = s.size()-1; i< s.size()/2; i++, j--){
        std::swap(s[i],s[j]);
    }
}
//反转字符串2--每计数至 2k 个字符，就反转这 2k 个字符中的前 k 个字符
//输入: s = "abcdefg", k = 2
//输出: "bacdfeg"
//思路：模拟 在遍历字符串的过程中
//只要让 i += (2 * k)，i 每次移动 2 * k 就可以了，然后判断是否需要有反转的区间。
std::string reverseString2(std::string s, int k) {
    for (int i = 0; i < s.size(); i += (2 * k)) {
        // 1. 每隔 2k 个字符的前 k 个字符进行反转
        // 2. 剩余字符小于 2k 但大于或等于 k 个，则反转前 k 个字符
        if (i + k <= s.size()) {
           std:: reverse(s.begin() + i, s.begin() + i + k );
        } else {
            // 3. 剩余字符少于 k 个，则将剩余字符全部反转。
            std::reverse(s.begin() + i, s.end());
        }
    }
    return s;
}
//替换数字--将字符串中的字母字符保持不变，而将每个数字字符替换为number
//输入：样例输入："a1b2c3"
//输出："anumberbnumbercnumber"
//思路：很多数组填充类的问题，其做法都是先预先给数组扩容带填充后的大小，然后在从后向前进行操作
std::string replaceNum(std::string s){
    int oldIndex = s.size()-1;
    int cnt = 0; //统计数字的个数
    for(int i = 0; i<s.size();i++){
        if(s[i]>='0'&&s[i]<='9'){
            cnt++;
        }
    }
    s.resize(s.size()+cnt*5);
    int newIndex = s.size()-1;
    //从后向前
    while(oldIndex >= 0){
        if(s[oldIndex]>='0' && &s[oldIndex]<='9'){
            s[newIndex--]='r';
            s[newIndex--]='e';
            s[newIndex--]='b';
            s[newIndex--]='m';
            s[newIndex--]='u';
            s[newIndex--]='n';
        }else{
            s[newIndex--] = s[oldIndex];
        }
        oldIndex--;
    }
    return s;
}
//反转字符串里面的单词
// 输入: "the sky is blue"
// 输出: "blue is sky the"
//思路：移除多余空格  "the sky is blue"
// 将整个字符串反转 "eulb si yks eht"
// 将每个单词反转 "blue is sky the"
//双指针
std::string removeSpace(std::string s){
    int slow = 0;
    for(int i = 0; i<s.size(); ++i){
        if(s[i]!=' '){
            if(slow!=0) s[slow++]=' ';
             while (i < s.size() && s[i] != ' ') { //补上该单词，遇到空格说明单词结束。
                s[slow++] = s[i++];
            }
        }
    }
    s.resize(slow); //slow的大小为去除多余空格后的大小
}
std::string reverseWords(std::string s) {
    removeExtraSpaces(s); //去除多余空格，保证单词之间之只有一个空格，且字符串首尾没空格。
    std::reverse(s, 0, s.size() - 1);
    int start = 0; //removeExtraSpaces后保证第一个单词的开始下标一定是0。
    for (int i = 0; i <= s.size(); ++i) {
        if (i == s.size() || s[i] == ' ') { //到达空格或者串尾，说明一个单词结束。进行翻转。
            std::reverse(s, start, i - 1); //翻转，注意是左闭右闭 []的翻转。
            start = i + 1; //更新下一个单词的开始下标start
        }
    }
    return s;
}
//右旋转字符串---把后面k个字符移到前面
//输入字符串 "abcdefg" 和整数
//函数应该将其转换为 "fgabcde"
//思路：两次倒叙
std::string reverseRightString(std::string s,int k){
    if(s.size()<k) return nullptr;
    std::reverse(s.begin, s.end());
    for(int i = 0, j = n; i<n/2; i++,j--){
        std::swap(s[i],s[j]);
    }
    std::reverse(s.begin()+n,s.end());
    return s;
}
//实现strStr/()---在 haystack 字符串中找出 needle 字符串出现的第一个位置 (从0开始)
// 输入: haystack = "hello", needle = "ll"
// 输出: 2
//思路：KMP算法--出现字符串不匹配时，可以记录一部分之前已经匹配的文本内容，
//利用这些信息避免从头再去做匹配
//next数组就是一个前缀表（prefix table）
//用来回退的，它记录了模式串与主串(文本串)不匹配的时候，模式串应该从哪里开始重新匹配
//前缀表：记录下标i之前（包括i）的字符串中，有多大长度的相同前缀后缀
//前缀是指不包含最后一个字符的所有以第一个字符开头的连续子串；
// 后缀是指不包含第一个字符的所有以最后一个字符结尾的连续子串

//构造next数组其实就是计算模式串s，前缀表的过程。 主要有如下三步：
// 初始化--两个指针i和j，j指向前缀末尾位置，i指向后缀末尾位置
int j = -1;
next[0] = j;
// 处理前后缀不相同的情况
for (int i = 1; i < s.size(); i++) 
// 处理前后缀相同的情况
void getNext(int* next, const string& s){
    int j = -1;
    next[0] = j;
    for(int i = 1; i < s.size(); i++) { // 注意i从1开始
        while (j >= 0 && s[i] != s[j + 1]) { // 前后缀不相同了
            j = next[j]; // 向前回退
        }
        if (s[i] == s[j + 1]) { // 找到相同的前后缀
            j++;
        }
        next[i] = j; // 将j（前缀的长度）赋给next[i]
    }
}
int strStr(string haystack, string needle) {
    if (needle.size() == 0) {
        return 0;
    }
    vector<int> next(needle.size());
    getNext(&next[0], needle);
    int j = -1; // // 因为next数组里记录的起始位置为-1
    for (int i = 0; i < haystack.size(); i++) { // 注意i就从0开始
        while(j >= 0 && haystack[i] != needle[j + 1]) { // 不匹配
            j = next[j]; // j 寻找之前匹配的位置
        }
        if (haystack[i] == needle[j + 1]) { // 匹配，j和i同时向后移动
            j++; // i的增加在for循环里
        }
        if (j == (needle.size() - 1) ) { // 文本串s里出现了模式串t
            return (i - needle.size() + 1);
        }
    }
    return -1;
}

//重复的子字符串--判断它是否可以由它的一个子串重复多次构成
//输入: "abab"
// 输出: True
// 解释: 可由子字符串 "ab" 重复两次构成。
//思路：KMP算法

void getNext (int* next, const string& s){
    next[0] = -1;
    int j = -1;
    for(int i = 1;i < s.size(); i++){
        while(j >= 0 && s[i] != s[j + 1]) {
            j = next[j];
        }
        if(s[i] == s[j + 1]) {
            j++;
        }
        next[i] = j;
    }
}
bool repeatedSubstringPattern (string s) {
    if (s.size() == 0) {
        return false;
    }
    int next[s.size()];
    getNext(next, s);
    int len = s.size();
    if (next[len - 1] != -1 && len % (len - (next[len - 1] + 1)) == 0) {
        return true;
    }
    return false;
}
