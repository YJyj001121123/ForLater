
#include <vector>
#include <algorithm>
#include <string>

/***
 * 数组
 * 连续内存空间 相同数据类型的集合
 * 删除增添 需要移动其他元素
 * C++中二维数组在地址空间上是连续的
 * 数组的元素在内存地址中是连续的，不能单独删除数组中的某个元素，只能覆盖。
 */
 //二分查找---返回目标的下标，无重复元素
 //输入：nums=[-1,0,3,5,9,12] target = 9
 //输出：4
 //思路：
int search(std::vector<int>& nums, int target){
    int left = 0;
    int right = nums.size()-1;
    while(left<=right){
        int mid = (left + right)/2;
        if (nums[mid] > target) {
            right = mid - 1; // target 在左区间，所以[left, middle - 1]
        } else if (nums[mid] < target) {
            left = mid + 1; // target 在右区间，所以[middle + 1, right]
        } else { // nums[middle] == target
            return mid; // 数组中找到目标值，直接返回下标
        }
    }
    return -1;

}
//搜索插入位置---如果不存在这个元素，返回它应该被插入的地方下标
// 输入: [1,3,5,6], 5
// 输出: 2
// 思路：二分
int searchInsert(std::vector<int>& nums, int val){
    int left = 0 ;
    int right = nums.size()-1;
    while(left<=right){
        int mid = (left+right)/2;
        if(nums[mid]>val) right = mid-1;
        else if (nums[mid]<val) left = mid+1;
        else return mid;
    }
    return right+1;
}
//排序数组查找元素的第一个和最后一个位置 
//输入：nums = [5,7,7,8,8,10], target = 8
//输出：[3,4]
int getRightBorder(std::vector<int>& nums, int target) {
    int left = 0;
    int right = nums.size() - 1;
    int rightBorder = -2; // 记录一下rightBorder没有被赋值的情况
    while (left <= right) {
        int middle = left + ((right - left) / 2);
        if (nums[middle] > target) {
            right = middle - 1;
        } else { // 寻找右边界，nums[middle] == target的时候更新left
            left = middle + 1;
            rightBorder = left;
        }
    }
    return rightBorder;
}
int getLeftBorder(std::vector<int>& nums, int target) {
    int left = 0;
    int right = nums.size() - 1;
    int leftBorder = -2; // 记录一下leftBorder没有被赋值的情况
    while (left <= right) {
        int middle = left + ((right - left) / 2);
        if (nums[middle] >= target) { // 寻找左边界，nums[middle] == target的时候更新right
            right = middle - 1;
            leftBorder = right;
        } else {
            left = middle + 1;
        }
    }
    return leftBorder;
}
std::vector<int> searchRange(std::vector<int>& nums, int target) {
    int leftBorder = getLeftBorder(nums, target);
    int rightBorder = getRightBorder(nums, target);
    // 情况一
    if (leftBorder == -2 || rightBorder == -2) return {-1, -1};
    // 情况三
    if (rightBorder - leftBorder > 1) return {leftBorder + 1, rightBorder - 1};
    // 情况二
    return {-1, -1};
}
//x的平方根
//输入：4
//输出：2
//思路：二分
int mySqrt(int x) {
    if (x <= 1) return x;
    int left = 0, right = x;
    while (left < right) {
        int mid = (right - left) / 2 + left;
        if (x / mid >= mid) left = mid + 1;
        else right = mid;
    }
    return right - 1;
}

//移除元素---移除val的元素
//输入：nums = [3,2,2,3], val = 3
//输出：2
//思路：快慢指针 
//快指针：寻找新数组的元素 ，新数组就是不含有目标元素的数组
//慢指针：指向更新 新数组下标的位置
int removeVal(std::vector<int>& nums, int val){
    int slowIndex = 0;
    for (int fastIndex = 0; fastIndex < nums.size(); fastIndex++){
        if(val != nums[fastIndex]){
            nums[slowIndex++] = nums[fastIndex];
        }
    }
    return slowIndex;
}
//删除有序数组的重复项
//输出：[1,2,2]
//输出：2 ---【1，2】
//思路：快慢指针
int removeDuplicates(std::vector<int>& nums){
    int slowIndex = 0;
    for(int fastIndex = 1; fastIndex < nums.size(); fastIndex++){
        if(nums[fastIndex]!=nums[fastIndex-1]){
            slowIndex++;
            nums[slowIndex] = nums[fastIndex];
        }
    }
    return slowIndex+1;
}
//移动零 -- 0移动到末尾
//输入：[0,1,0,3,13]
//输出：[1,3,13,0,0]
//思路：快慢指针
int moveZero(std::vector<int>& nums){
    int slowIndex = 0;
    for(int fastIndex = 0; fastIndex<nums.size(); fastIndex++){
        if(nums[fastIndex]!=0){
            nums[slowIndex++] = nums[fastIndex];
        }
    }
    // 将slowIndex之后的冗余元素赋值为0
    for (int i = slowIndex; i < nums.size(); i++) {
        nums[i] = 0;
    }
}
//比较含退格的字符串--#代表退格
//输入：s = "ab#c", t = "ad#c"
// 输出：true
// 解释：s 和 t 都会变成 "ac"。
//
std::string backSpace(std::string& s1){
    int slow = 0;
    std::string str;
    for(int fast = 0;fast<s1.size();fast++){
        if(s1[fast] != '#'){
            str[slow++] = str[fast];
        }else{
            if(slow>0 ){
                slow--;
            }
        }
    }
    return str;
}
bool compare(std::string& s1, std::string& s2){
    if(backSpace(s1)==backSpace(s2)) return true;
    return false;  
}
//有序数组的平方---有序
//输入： [-4,-1,0,3,10]
//输出： [0,1,9,16,100]
//思路： 双指针--需要从两端入手
std::vector<int> SquareSort(std::vector<int>& nums){
    std::vector<int> res(nums.size());
    int left = 0;
    int right = nums.size()-1;
    int index = right;
    while(left<=right){
        if(nums[left] * nums[left] > nums[right] * nums[right]){
            res[index--]=nums[left] * nums[left] ;
            left++;
        }else {
            res[index--] = nums[right] * nums[right];
            right--;
        }
    }
    return res;
}
//长度最小的子数组 -- 满足s和的最短
//输入：s = 7, nums = [2,3,1,2,4,3]
//输出：2
//解释：子数组 [4,3] 是该条件下的长度最小的子数组。
//思路：滑动窗口--不断的调节子序列的起始位置和终止位置，从而得出我们要想的结果。
int minSubArray(std::vector<int>& nums, int target){
    int res = INT32_MAX;
    int sum = 0; // 滑动窗口数值之和
    int i = 0; // 滑动窗口起始位置
    int subLength = 0; // 滑动窗口的长度
    for (int j = 0; j < nums.size(); j++) {
        sum += nums[j];
        // 注意这里使用while，每次更新 i（起始位置），并不断比较子序列是否符合条件
        while (sum >= target) {
            subLength = (j - i + 1); // 取子序列的长度
            res = res < subLength ? res : subLength;
            sum -= nums[i++]; // 这里体现出滑动窗口的精髓之处，不断变更i（子序列的起始位置）
        }
    }
    // 如果result没有被赋值的话，就返回0，说明没有符合条件的子序列
    return res == INT32_MAX ? 0 : res;
}
//最小覆盖子串--寻找ABC覆盖的最小子串
//输入：s="ADOBECODEBANC" t="ABC"
//输出：“BANC"
std::string minSubString(std::string s, std::string t){
    std::unordered_map<char,int>hs,ht;
    for(auto c:t) ht[c]++;
    std::string res;
    int cnt = 0;
    for(int i = 0,j=0; i<s.size();i++){
        hs[s[i]]++;
        if(hs[s[i]]<=ht[s[i]]) cnt++;
        while(hs[s[j]>ht[s[j]]]) hs[s[j++]]--;
        if(cnt == t.size()){
            if(res.empty()||i-j+1<res.size()) res = s.substr(j,i-j+1);
        }
    }
    return res;
}
//螺旋矩阵2---生成按顺时针排序的矩阵 包含1到n的平方
//输入: 3 
//输出: [ [ 1, 2, 3 ], [ 8, 9, 4 ], [ 7, 6, 5 ] ]
//思路：模拟行为
//模拟顺时针画矩阵的过程:
// 填充上行从左到右
// 填充右列从上到下
// 填充下行从右到左
// 填充左列从下到上
std::vector<int> gengerateMatrix(int n){
    std::vector<std::vector<int>> res(n, std::vector<int>(n, 0)); // 使用vector定义一个二维数组
    int startx = 0, starty = 0; // 定义每循环一个圈的起始位置
    int loop = n / 2; // 每个圈循环几次，例如n为奇数3，那么loop = 1 只是循环一圈，矩阵中间的值需要单独处理
    int mid = n / 2; // 矩阵中间的位置，例如：n为3， 中间的位置就是(1，1)，n为5，中间位置为(2, 2)
    int count = 1; // 用来给矩阵中每一个空格赋值
    int offset = 1; // 需要控制每一条边遍历的长度，每次循环右边界收缩一位
    int i,j;
    while (loop --) {
        i = startx;
        j = starty;
        // 下面开始的四个for就是模拟转了一圈
        // 模拟填充上行从左到右(左闭右开)
        for (j; j < n - offset; j++) {
            res[i][j] = count++;
        }
        // 模拟填充右列从上到下(左闭右开)
        for (i; i < n - offset; i++) {
            res[i][j] = count++;
        }
        // 模拟填充下行从右到左(左闭右开)
        for (; j > starty; j--) {
            res[i][j] = count++;
        }
        // 模拟填充左列从下到上(左闭右开)
        for (; i > startx; i--) {
            res[i][j] = count++;
        }
        // 第二圈开始的时候，起始位置要各自加1， 例如：第一圈起始位置是(0, 0)，第二圈起始位置是(1, 1)
        startx++;
        starty++;
        // offset 控制每一圈里每一条边遍历的长度
        offset += 1;
    }
     // 如果n为奇数的话，需要单独给矩阵最中间的位置赋值
    if (n % 2) {
        res[mid][mid] = count;
    }
    return res;
}

//螺旋矩阵
//输入：[1,2,3]
//     [4,5,6]
//     [7,8,9]
//输出：[1，2，3，6，9，8，7，4，5]
//思路：多指针+需要四个方向，上下左右走，难点其实就在于各个方向边界的判断。上右下左一次遍历，注意更新边界条件即可。
std::vector<int> sporalOrder(std::vector<std::vector<int>>& matrix){
    std::vector<int> res;
    int m = matrix.size();
    int n = matrix[0].size();
    int top = 0;
    int bottom = m -1;
    int left = 0;
    int right = n -1;
    while(true){
        for(int col = left; col <= right; ++col) res.emplace_back(matrix[top][col]);
        ++top;
        if(top>bottom) break;

        for(int row = top; row <= bottom; ++row) res.emplace_back(matrix[row][right]);
        --right;
        if(right<left) break;

        for(int col = right; col >= left; --col) res.emplace_back(matrix[bottom][col]);
        --bottom;//右边界左移
        if(top > bottom) break;

        for(int row = bottom; row >= top; --row) res.emplace_back(matrix[row][left]);
        ++left;//右边界左移
        if(right < left) break;
    }
    return res;
}

