// #include "pch.h" // Use with pre-compiled header
#include <thread>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <list>
#include <stack>
#include <unordered_set>
#include <queue>

////多线程
int count = 0;
void doSomeWork() {
    std::cout << "The doSomeWork function is running on another thread." << std::endl;
    int data = count++;
    // Pause for a moment to provide a delay to make
    // threads more apparent.
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::string str = std::to_string(data);
    std::cout << "The function called by the worker thread has ended. " + str<< std::endl;
}
void runTest() {
    std::vector<std::thread> threads;
    for (int i = 0; i < 10; ++i) {
        threads.push_back(std::thread(doSomeWork));
        std::cout << "The Main() thread calls this after starting the new thread" << std::endl;
    }
    for (auto& thread : threads) {
        thread.join();
    }
}

////sizeof
class CppBasic {
};
class CppBasic2 {
public:
    CppBasic2(){};
    ~CppBasic2(){};
};
class CppBasic3 {
public:
    CppBasic3(){};
    virtual ~CppBasic3(){};
};
void sizeofTest() {
    // 空类型 不同的声明方式 sizeof 占不同个字节 编译器决定。 实例占用内存。
    std::shared_ptr<CppBasic> cb_ = std::make_shared<CppBasic>(); //16
    auto cb = new CppBasic(); //8
    CppBasic cp1;  //1
    auto null_size = sizeof (cp1);
//    std::cout << null_size << std::endl;
    //含有析构和前面返回一样。只需要知道地址即可，与实例无关。
    std::shared_ptr<CppBasic2> cc = std::make_shared<CppBasic2>(); //16
    auto ccc = new CppBasic2(); //8
    CppBasic2 cccc; //1
    auto null = sizeof (cccc);
//    std::cout << null << std::endl;
    //析构为虚析构，需要维护虚表（对应的指针占用）32位机器 一个指针占4个字节 64位占8个字节
    std::shared_ptr<CppBasic3> v_c = std::make_shared<CppBasic3>(); //16
    auto v_cc = new CppBasic3(); //8
    CppBasic3 v_ccc; // 8
    auto null3 = sizeof (v_ccc);
    std::cout << null3 << std::endl;
}

////复制构造
class copy{
public:
    copy(int n) {
        value = n;
    }
    //不允许出现复制构造函数传值参数，要使用常量引用
    copy(const copy& ins) {
        value = ins.value;
    }
private:
    int value;
};

////赋值运算符
class CMyString{
public:
    CMyString(char * data = nullptr);
    CMyString(const CMyString* str);
    CMyString& operator=(const CMyString& str);
private:
    char * m_data;
};
    //只有返回*this,才可以连续赋值，释放内存
CMyString &CMyString::operator=(const CMyString &str) {
    if(this != & str){
        CMyString te(str);
        char * temp = te.m_data;
        te.m_data = m_data;
    }
    return *this;
}
    // struct 默认public class 默认private

////Singleton mode
    //饿汗 static 启动完成构造
class Singleton {
public:
    static Singleton* getInstance() {
        return singleton_;
    }
    static void destroyInstance() {
        if (singleton_ != NULL) {
            delete singleton_;
        }
    }
private:
    Singleton() = default;
    // 防止拷贝和赋值。
    Singleton& operator=(const Singleton&) = delete;
    Singleton(const Singleton& singleton2) = delete;
private:
    static Singleton* singleton_;
};
    //延迟构造
class Singleton2 {
public:
    static Singleton2& GetInstance() {
        if (!instance_) {
            std::lock_guard<std::mutex> lock(mutex_);
            if (!instance_) {
                instance_.reset(new Singleton2);
            }
        }
        return *instance_;
    }
    ~Singleton2() = default;
private:
    Singleton2() = default;
    Singleton2(const Singleton&) = delete;
    Singleton2& operator=(const Singleton2&) = delete;
private:
    static std::unique_ptr<Singleton2> instance_;
    static std::mutex mutex_;
};
    //std::call_once 保证函数或者一些代码段在并发或者多线程的情况下，始终只会被执行一次
class Singleton3 {
public:
    static Singleton3& GetInstance() {
        static std::once_flag s_flag;
        std::call_once(s_flag, [&]() {
            instance_.reset(new Singleton3);
        });
        return *instance_;
    }
    ~Singleton3() = default;
private:
    Singleton3() = default;
    Singleton3(const Singleton3&) = delete;
    Singleton3& operator=(const Singleton3&) = delete;
private:
    static std::unique_ptr<Singleton3> instance_;
};

////数组
    //内存连续 o(1) 空间效率不高 动态数组vector
    //数组的name 是一个指针，指向第一个元素
    //排序o(nlogn)
//找重复数字 eg.[1,2,3,2,3,4] ---> 2,3
std::vector<int> repeatNum(std::vector<int>& nums){
    std::vector<int> res_;
    if(nums.empty()) return res_;
    for(int i = 0 ; i < nums.size() - 1; i++){
        auto temp = nums[i];
        while ( temp != i) {
            if(temp == nums[temp]){
                res_.emplace_back(temp);
            } else{
                nums[i] = nums[temp];
                nums[temp] = temp;
            }
        }
    }
    return res_;
}
std::vector<int> repeatNum2(std::vector<int>& nums){
    std::vector<int> res_;
    std::map<int, int> help_;
    if(nums.empty()) return res_;
    for(int i = 0 ; i < nums.size() - 1; i++){
        auto item = nums[i];
        help_[item] = item;
        if(help_.count(item) == 0){
            res_.push_back(item);
        }
    }
    return res_;
}
//二维数组查找：每行每列递增，寻找是否存在整数
bool findNum(std::vector<std::vector<int>>& nums, int target) {
    int size_hang = nums.size() - 1;
    int size_lie = nums[0].size() - 1;
    if (size_hang != 0 && size_lie != 0) return false;
    while (int r = 0 < size_hang && size_lie > 0) {
        if (target < nums[r][size_lie]) {
            --size_lie;
        } else if (target == nums[r][size_lie]) {
            return true;
            break;
        } else {
            ++r;
        }
    }
    return false;
}
//调整数组顺序，奇数位于偶数前面
//快慢指针交换即可
std::vector<int> reorderNums(std::vector<int>& nums){
    std::vector<int> res;
    if(nums.empty()) return res;
    int start = 0;
    int end = nums.size()-1;
    while(start < end){
        while(start < nums.size()&& nums[start]%2 ==1){
            ++start;
        }
        while((end & 0x1) == 0) {
            end--;
        }
        if(start < end) std::swap(nums[start], nums[end]);
    }


}

////字符串
    //字符以'\0'字符结尾，需要额外字符的开销 谨防越界
    //节省内存，C++放到单独一个内区域 几个指针指向相同字符串常量 指向相同地址（char[]不同 和 char*相同）
//替换空格 eg."We are happy"--->"We%20are%20happy"
//空格ASCII32 0x20 URL替换%20 #ASCII35 0x23 替换%23
std::string replaceBlank(std::string str){
    if(str.empty()) return nullptr;
    int size = str.length();
    int space_count = 0;
    for(auto ch :str){
        if(ch == ' '){
            ++space_count;
        }
    }
    int newSize = size+2*space_count;
    str.resize(newSize);
    int noChangeIndex = size;
    int changeIndex = newSize;
    while (noChangeIndex >= 0 && changeIndex > noChangeIndex)
    {
        if (str[noChangeIndex] == ' ') {
            str[changeIndex--] = '0';
            str[changeIndex--] = '2';
            str[changeIndex--] = '%';
        } else {
            str[changeIndex--] = str[noChangeIndex];
        }
        --noChangeIndex;
    }
    return str;
}
////链表
    //动态数据结构，每次为新节点分配内存，不是一次性分配好的
    //std::list <xx> front 头部元素 back 末尾元素 pop_front 删除头部 pop_back
    // assign 替换 insert指定位置插入 push_front push_back
struct ListNode {
    int m_value_;
    ListNode* p_next_;
    ListNode() : m_value_(0), p_next_(nullptr) {}
    ListNode(int x) : m_value_(x), p_next_(nullptr) {}
};
//反置列表 {1,2,3}
ListNode* reverseList(ListNode* head) {
    ListNode* pCurrent = head;
    ListNode* pPre =  nullptr;
    while(pCurrent) {
        ListNode* pNext = pCurrent ->p_next_;
        pCurrent->p_next_ = pPre;
        pPre = pCurrent;
        pCurrent = pNext;
    }
    std::list<int> list_;
    list_.reverse();
    return pPre;
}
//删除链表节点 o(1)时间内
//顺序查找o(n)
//删除下一个 把值复制给当前
void deleteListNode(ListNode* head, ListNode*dele){
    if(head == nullptr || dele == nullptr) return;
    //非尾节点
    if(dele->p_next_ != nullptr){
        ListNode* pNext = dele->p_next_;
        dele->m_value_ = pNext->m_value_;
        dele->p_next_ = pNext->p_next_;
        delete pNext;
        pNext = nullptr;
        //头节点
    } else if (head == dele){
        delete dele;
        head == nullptr;
        dele == nullptr;
    } else{ //尾
        ListNode* PNode = head;
        while(PNode->p_next_ != dele){
            PNode = PNode->p_next_;
        }
        PNode->p_next_ = nullptr;
        delete dele;
        dele = nullptr;
    }
}
//删除重复节点
void delteDup(ListNode* head){
    if(head == nullptr) return;
    ListNode* pre = nullptr;
    ListNode* pCurrent = head;
    while (pCurrent != nullptr){
        ListNode* pNext = pCurrent->p_next_;
        if(pNext != nullptr && pNext->m_value_ == pCurrent->m_value_) {
            int val = pCurrent->m_value_;
            ListNode* pDeLe = pNext;
            while(pDeLe != nullptr && pDeLe->m_value_ == val){
                pNext=pDeLe->p_next_;
                delete (pDeLe);
                pDeLe = nullptr;
                pDeLe = pNext;
            }
            if(pre == nullptr) head = pNext;
            else pre->p_next_ = pNext;
            pCurrent = pNext;
        } else {
            pre=pCurrent;
            pCurrent = pCurrent->p_next_;
        }
    }

}
//输出倒数第k个节点
//只遍历一遍 双指针 差k 一个走到尾节点即可
ListNode* FindK(ListNode* head, int k){
    if(head == nullptr || k == 0) return nullptr;
    ListNode* pCurrent = head;
    ListNode* pPre = nullptr;
    for(int i = 0 ; i < k; i++){
        if(pCurrent->p_next_ != nullptr){
            pCurrent = pCurrent->p_next_;
        } else {
            return nullptr;
        }
    }
    pPre = head;
    while(pCurrent->p_next_ != nullptr){
        pCurrent = pCurrent->p_next_;
        pPre = pPre->p_next_;
    }
    return pPre;
}
//链表环的入口节点
//快慢指针找环 n个节点 相差n去走
ListNode* MeetingNode(ListNode* head){
    if(head == nullptr) return nullptr;
    ListNode* low = head;
    ListNode* fast = head;
    while(fast->p_next_ && fast->p_next_->p_next_){
        low = low->p_next_;
        fast = fast->p_next_->p_next_;
        if(low == fast) return fast;
    }
    return nullptr;
}
ListNode* EntryNodeOfLoop(ListNode* head){
    ListNode* meetingNode = MeetingNode(head);
    if(meetingNode == nullptr) return nullptr;
    // 得到环中结点的数目
    int numsInLoop = 1;
    ListNode* node1 = meetingNode;
    while(node1->p_next_ != meetingNode){
        node1 = node1->p_next_;
        ++numsInLoop;
    }
    // 先移动pNode1，次数为环中结点的数目
    node1 = head;
    for(int i = 0; i < numsInLoop; ++i) node1 = node1->p_next_;
    // 再移动pNode1和pNode2
    ListNode* node2 = head;
    while(node1 != node2){
        node1 = node1->p_next_;
        node2 = node2->p_next_;
    }
    return node1;
}
//反转
ListNode *reverselList(ListNode *A) { //{1,2,3}  { 2,3，4} { ,2,1}
    ListNode *cur_ptr_ = A;
    ListNode *pre_ptr = nullptr;
    while (cur_ptr_) {
        ListNode *next_ptr_ = cur_ptr_->p_next_; //3
        cur_ptr_->p_next_ = pre_ptr;  //
        pre_ptr = cur_ptr_;
        cur_ptr_ = next_ptr_;
    }
    return pre_ptr;
}
//合并两个递增排序的链表 仍旧递增
ListNode* MergeList(ListNode* headA, ListNode* headB){
    if(headA == nullptr || headB == nullptr) return nullptr;
    ListNode* pMerageHead = nullptr;
    if(headA->m_value_ < headB->m_value_) {
        pMerageHead = headA;
        pMerageHead->p_next_ = MergeList(headA->p_next_, headB);
    } else{
        pMerageHead=headB;
        pMerageHead->p_next_ = MergeList(headA, headB->p_next_);
    }
}
////树
    //除根结点外 每个节点只有一个父节点。 指针操作
    //二叉树 前序遍历--根左右 中序--左根右 后序--左右根 层序 递归 循环
    //广度优先遍历 深度优先遍历
    //二叉搜索树 左结总是小于或等于根结点 右节点大于或等于根节点 o(logn)
    //堆 最大堆--根节点最大 最小堆--根结点最小
    //红黑树 根结点到叶结点最长路径的长度不超过最短路径的两倍 std::set map
struct TreeNode {
    int mValue;
    TreeNode* pLeft;
    TreeNode* pRight;
    TreeNode* pRoot;
    TreeNode(int x) : mValue(x), pLeft(nullptr), pRight(nullptr), pRoot(nullptr) {}
};
//重建二叉树 输入前、中序遍历结果。重建二叉树 递归
TreeNode *reConstructBinaryTree(std::vector<int> pre, std::vector<int> vin) {
    if (pre.empty() && vin.empty())     // 树为空
        return nullptr;
    auto *root = new TreeNode(pre[0]);     // 前序遍历的第一个结点为根结点
    // 在中序遍历中找到根结点的位置
    auto mid = find(vin.begin(),vin.end(),root->mValue);
    std::vector<int> leftin{vin.begin(),mid};       // 左子树的中序遍历序列
    std::vector<int> rightin{mid+1,vin.end()};      // 右子树的中序遍历序列
    int nleft = leftin.size();            // 左子树中的结点个数
    int nright = rightin.size();          // 右子树中的结点个数
    std::vector<int> leftpre {pre.begin()+1,pre.begin()+nleft+1};   // 左子树的前序遍历序列
    std::vector<int> rightpre {pre.end()-nright,pre.end()};         // 右子树的前序遍历序列
    if (nleft)      // 左子树不为空
        root->pLeft = reConstructBinaryTree(leftpre,leftin);     // 递归构建左子树
    if (nright)     // 右子树不为空
        root->pRight = reConstructBinaryTree(rightpre,rightin);  // 递归构建右子树
    return root;    // 返回树根
}
//二叉树下一个节点  给定一个二叉树 和其中一个节点 找出中序的下一个节点
//如果节点有右子节点，则右子节点的最左节点是该节点的下一个节点。例如，寻找b的下一个节点的过程（b有右子节点e，e的左子节点是h，且h是e的最左节点，h是b的下一个节点）
//如果节点无右子节点，但该节点是父节点的左子节点，则父节点是该节点的下一个节点。例如，寻找d的下一个节点的过程（d无右子节点，d是父节点b的左子节点，则b是de的下一个节点）
//如果节点无右子节点，且该节点是父节点的右子节点，则沿着父节点的指针向上遍历。例如，寻找i的下一个节点的过程（i的父节点e，e是其父节点b的右子节点，节点b是其父节点a的左子节点，节点a是节点i的下一个节点）
TreeNode* GetNext(TreeNode* pNode)
{
    if(pNode == nullptr) return nullptr;
    TreeNode* pNext = nullptr;
    if(pNode->pRight != nullptr){
        TreeNode* p_right = pNode->pRight;
        while(p_right->pLeft != nullptr){
            p_right = p_right->pLeft;
        }
        pNext = p_right;
    } else if(pNode->pRoot != nullptr){
        TreeNode* p_current = pNode;
        TreeNode* p_root = pNode->pRoot;
        while(p_root != nullptr && p_current == p_root->pRight){
            p_current = p_root;
            p_root = p_root->pRoot;
        }
        pNext = p_root;
    }
    return  pNext;
}
//树的子结构 判断B是不是A的子结构
//A上找与B的根结点 再去看子树
bool recur(TreeNode* A, TreeNode* B){
    if(B == nullptr) return true;
    if(A == nullptr || A->mValue != B->mValue) return false;
    return recur(A->pLeft, B->pLeft) && recur(A->pRight, B->pRight);
}
bool isSubStructure(TreeNode* A, TreeNode* B) {
    if(A == nullptr) return false;
    if(B == nullptr) return false;
    return (recur(A, B) || isSubStructure(A->pLeft, B) || isSubStructure(A->pRight, B));
}
////栈和队列
    //栈 存储函数调用各个函数的参数，返回地址及临时变量。 后进先出 o(n)
    //std::stack top访问栈顶
    //队列 先进先出
//两个栈实现队列 队列尾部插入节点 队列头部删除节点
template <typename T>
class CQueue{
public:
    CQueue();
    ~CQueue();
    void addTail(const T& node);
    T deleHead();
private:
    std::stack<T> stack1;
    std::stack<T> stack2;
};
template<typename T>void CQueue<T>::addTail(const T &node) {
    stack1.push(node);
}
template<typename T>T CQueue<T>::deleHead(){
    if(stack2.size() <= 0){
        while(stack1.size() > 0){
            T& data = stack1.top();
            stack2.push(data);
        }
    }
    if(stack2.size() == 0) throw new std::exception;
    T head = stack2.pop();
    stack2.pop();
    return  head;
}
////算法--递归和循环
    //递归在函数内部调用自身。 缺点函数调用用时间空间消耗，每一次函数调用，都需要内存栈分配空间保存参数、返回地址和临时变量
    //栈溢出。
//斐波那契数列 动态规划
int Fib(unsigned n){
    if(n<2) return n;
    int FibOne = 1;
    int FibTwo = 0;
    int FibN =0;
    for (unsigned int i = 2; i <= n; ++i){
        FibN = FibOne + FibTwo;
        FibTwo = FibOne;
        FibOne = FibN;
    }
    return FibN;
//    int Mod = 1000000007;
//    if(n<2){
//        return n;
//    }
//    int p = 0;
//    int q= 0;
//    int r = 1;
//    for(int i =2; i<=n; ++i){
//        p = q;
//        q = r;
//        r = (p+q)%Mod;
//    }
//    return r;
}
////算法--查找和排序
    //查找 二分 hash o(1) 冒泡、归并、快排
//快排
int paritition(std::vector<int> nums, int low, int high){
    if(nums.empty() || low < 0 || high >= nums.size()) return -1;
    int index = nums[low];
    while(low < high){
        while (nums[high] >= index) {
            --high;
        }
        nums[low] = nums[high];
        while (nums[low] <= index) {
            ++low;
        }
        nums[high] = nums[low];
    }
    nums[low] = index;
    return low;
}

void QuickSort(std::vector<int> n, int low, int high) //快排母函数
{
    if (low < high) {
        int pivot = paritition(n, low, high);
        QuickSort(n, low, pivot - 1);
        QuickSort(n, pivot + 1, high);
    }
}
//旋转数组的最小数字  {3,4,5,1,2}-->{1,2,3,4,5} 二分
int Min(std::vector<int> nums){
    if(nums.empty()) return  -1;
    int index1 = 0;
    int index2 = nums.size()-1;
    int  indexMid = index1;
    while(nums[index1] >= nums[index2]){
        if(index2-index1 ==1) {
            indexMid = index2;
            break;
        }
        indexMid = (index1 + index2)/2;
        if(nums[indexMid] >= nums[index2]) index1 = indexMid;
        else if(nums[indexMid] <= nums[index2]) index2 = indexMid;
    }
    return nums[indexMid];
}
////算法--回溯法 xxxxx
    //适合多个步骤组成的问题，并且每个步骤有多个选项。
//矩阵中的路径 判断矩阵是否包含一条包含某字符串所有字符的路径。路径可以从任意开始，可以全方位走一格
//路径可以看为栈
bool dfs(std::vector<std::vector<char>>& b, std::string& w, int i, int j, int k) {
    if (i >= b.size() || i < 0 || j >= b[0].size() || j < 0 || b[i][j] != w[k])
        return false;
    if (k == w.length() - 1)
        return true;
    char temp = b[i][j];
    b[i][j] = '/';
    bool res = dfs(b, w, i + 1, j, k + 1) ||
               dfs(b, w, i - 1, j, k + 1) ||
               dfs(b, w, i, j + 1, k + 1) ||
               dfs(b, w, i, j - 1, k + 1);
    /*
    可以这么写，比较好：
    int dx[4] = {-1, 0, 1, 0}, dy[4] = {0, 1, 0, -1};
    for (int q = 0; q < 4; q ++ ) {
        int m = i + dx[q], n = j + dy[q];
        if (dfs(b, w, m, n, k + 1)) return true;
    }
    */
    b[i][j] = temp;
    return res;
}
bool exist(std::vector<std::vector<char>>& board, std::string word) {
    for (int i = 0; i < board.size(); i++) {
        for (int j = 0; j < board[0].size(); j++) {
            if (dfs(board, word, i, j, 0))
                return true;
        }
    }
    return false;
}
////算法--动态规划与贪婪算法
    //求最优解 问题分为若干个自问题--动态规划
    //贪婪选择
//剪绳子 长度n，剪成m段。每段为k[0],k[1],...k[m] 的最大乘积？pow 求幂
//函数f(n) = max(f(i)*f(n-i))
int maxDynamic(int length){
    if(length<2) return 0;
    if(length==2) return 1;
    if(length==3) return 2;
    std::vector<int> products(length+1);
    products[0] = 0;
    products[1] = 1;
    products[2] = 2;
    products[3] = 3;
    int max = 0;
    for(int i = 4; i <= length; ++i){
        max = 0;
        for(int j = 1; j <= i/2; ++j){
            int product = products[j]*products[i-j];
            if(max < product) max = product;
            products[i] = max;
        }
    }
    max = products[length];
    return max;
}
int cuttingRope(int n) {
    if(n == 0) return 0;
    std::vector<int> dp(n + 1, 0);
    dp[1] = 1;
    dp[2] = 1;
    for(int i = 3; i <= n; i++) {
        for(int k = 2; k <= i - 1; k++) {
            //k*(i-k)表示剪成两段，而k*dp[i-k]表示将i-k段继续剪
            dp[i] = std::max(dp[i], std::max(dp[i - k]*k, k*(i-k)));
        }
    }
    return dp[n];
}
int cuttingRopeBag(int n) {
    if(n == 0) return 0;
    std::vector<int> dp(n + 1, 0);
    dp[0] = 1;
    for(int i = 1; i <= (n + 1)/2; i++)
    {
        for(int j = i; j <= n; j++)
        {
            dp[j] = std::max(dp[j], dp[j - i] * i);
        }
    }
    return dp[n];
}
int MaxTanlan(int length) {
    if(length<2) return 0;
    if(length==2) return 1;
    if(length==3) return 2;
    //多剪长度为三的
    int piece_three = length/3;
    //当剩余4时候，不剪3，剪2*2
    if(length-piece_three *3 == 1) piece_three -=1;
    int piece_two = (length-piece_three*3)/2;
    return pow(3, piece_three) * pow(2,piece_two);
}
////算法--位运算
    // 0 & 0 = 0/ 0 & 1 = 0/ 1 & 0 =0/ 1 & 1 =1
    // 0 ｜ 0 = 0/ 0 ｜ 1 = 1/ 1 ｜ 0 =1/ 1 ｜ 1 = 0
    // 0 ^ 0 = 0/ 0 ^ 1 = 1/ 1 ^ 0 = 1/ 1 ^ 1 = 1
    //左移  m<<n m移动n位 左边n位丢弃，右边补0
    //右移  m>>n 右边丢弃，无符号数左0补，符号数数字--正数补0，负数补1
    //除法效率不如移位运算
//二进制中1的个数 eg. 9 --- 1001 两个1
int oneNum(int n){
    int count = 0 ;
    while(n) {
//        if(n&1) count++;
//        n=n>>1;
        ++count;
        n = (n-1)&n;
    }
    return count;
}
////代码质量和鲁棒性
//打印从1到最大的n位数。 eg 输入3 打印1，2，3到最大999
//字符串或数组表达大数 n位 n+1长度
    //ASCII
    //用一个char与一个int相加，我们会得到一个char。比如：‘0’ + 1 = 48 + 1 = 49 = ‘1’。
    //用一个char减去另一个char，我们会得到一个int。比如：‘2’ - ‘0’ = 50 - 48 = 2。
    //一个int型的数（比如 i = 5）转成char型，我们可以写成：char c = i + ‘0’
    //一个char型的数（比如 c = ‘5’）转成int型，我们可以写成：int i = c - ‘0’。
    // 字符串number表示一个数字，在 number上增加1
    // 如果做加法溢出，则返回true；否则为false
bool Increment(char* number){
    bool isOverflow = false;
    int nTakeOver = 0;
    int nLength = strlen(number);
    for (int i = nLength - 1; i >= 0; i--){
        int nSum = number[i] - '0' + nTakeOver;
        if (i == nLength - 1)
            nSum++;
        if (nSum >= 10){
            if (i == 0) isOverflow = true;
            else{
                nSum -= 10;
                nTakeOver = 1;
                number[i] = '0' + nSum;
            }
        }
        else
        {
            number[i] = '0' + nSum;
            break;
        }
    }
    return isOverflow;
}
void printStrNum(char* num) {
    int i = 0;
    while (num[i] == '0') {
        i++;
    }
    while (num[i] != '\0') {
        std::cout << num[i];
        i++;
    }
    std::cout << ' ';
}
    //打印部分
void Print1ToMaxOfNDigits_1(int n){
    if (n <= 0)
        return;
    char *number = new char[n + 1];
    memset(number, '0', n);//初始化函数
    number[n] = '\0';
    while (!Increment(number)){
        printStrNum(number);
    }
    delete[]number;
}
    //不考虑大数字
std::vector<int> printNumbers(int n) {
    std::vector<int>v;
    for(int i=1;i<pow(10,n);i++)
    {
        v.push_back(i);
    }
    return v;
}
//表示数值的字符串 判断输入的字符串是不是数值 例如“1.2.3”不算
bool scanUnsignedInteger(const std::string s, int& index){
    int befor = index;
    while(index != s.size() && s[index] >= '0' && s[index] <= '9') index ++;
    return index > befor;
}
    // 整数的格式可以用[+|-]B表示, 其中B为无符号整数
bool scanInteger(const std::string s, int& index){
    if(s[index] == '+' || s[index] == '-') ++index;
    return scanUnsignedInteger(s, index);
}
    // 数字的格式可以用A[.[B]][e|EC]或者.B[e|EC]表示，
    // 其中A和C都是整数（可以有正负号，也可以没有），而B是一个无符号整数
bool isNumber(std::string s){
    if(s.size() == 0) return false;
    int index = 0;
    //字符串开始有空格，可以返回true
    while(s[index] == ' ') ++index;
    bool numeric = scanInteger(s, index);
    // 如果出现'.'，接下来是数字的小数部分
    if(s[index] == '.'){
        ++index;
        // 下面一行代码用||的原因：
        // 1. 小数可以没有整数部分，例如.123等于0.123；
        // 2. 小数点后面可以没有数字，例如233.等于233.0；
        // 3. 当然小数点前面和后面可以有数字，例如233.666
        numeric = scanUnsignedInteger(s, index) || numeric;
    }
    // 如果出现'e'或者'E'，接下来跟着的是数字的指数部分
    if(s[index] == 'e' || s[index] == 'E'){
        ++index;
        // 下面一行代码用&&的原因：
        // 1. 当e或E前面没有数字时，整个字符串不能表示数字，例如.e1、e1；
        // 2. 当e或E后面没有整数时，整个字符串不能表示数字，例如12e、12e+5.4
        numeric = numeric && scanInteger(s ,index);
    }
    //字符串结尾有空格，可以返回true
    while(s[index] == ' ') ++index;
    return numeric && index == s.size();
}
////手动实现shared_ptr
//加上explict 防止指针隐式转换
//传入空指针或者使用默认构造函数，引用计数为0
//拷贝赋值函数需要检查自赋值的情况
//释放动态内存主要指针是不是空指针


////std::function std::bind
class TestA{
public:
    void FA1 () {
        printf("Hello FA1");
    }
    void FA2 () {
        printf("Hello FA2");
    }
};
class TestB{
    typedef std::function<void ()> CallbackFunction;
public:
    void FB1(CallbackFunction callback){
        callback();
    };
};

std::function< int(int)> Functional;
// 普通函数
int TestFunc(int a)
{
    return a;
}
// Lambda表达式
auto lambda = [](int a)->int{ return a; };

// 仿函数(functor)
class Functor
{
public:
    int operator()(int a)
    {
        return a;
    }
};

// 1.类成员函数
// 2.类静态函数
class TestClass
{
public:
    int ClassMember(int a) { return a; }
    static int StaticMember(int a) { return a; }
};

void funCall()
{
    // 普通函数
    Functional = TestFunc;
    int result = Functional(10);
    std::cout << "普通函数："<< result << std::endl;

    // Lambda表达式
    Functional = lambda;
    result = Functional(20);
    std::cout << "Lambda表达式："<< result << std::endl;

    // 仿函数
    Functor testFunctor;
    Functional = testFunctor;
    result = Functional(30);
    std::cout << "仿函数："<< result << std::endl;

    // 类成员函数
    TestClass testObj;
    Functional = std::bind(&TestClass::ClassMember, testObj, std::placeholders::_1);
    result = Functional(40);
    std::cout << "类成员函数："<< result << std::endl;

    // 类静态函数
    Functional = TestClass::StaticMember;
    result = Functional(50);
    std::cout << "类静态函数："<< result << std::endl;

    TestB b;
    TestA a;
    b.FB1(std::bind(&TestA::FA2, &a));
}

//// 05.06
//无重复字符的最长子串
//输入：“abcabcbb"
//输出：“abc" length=3
//输入：“bbbbb"
//输出：“b" length=1
//思路：滑动窗口 窗口大小不固定
int lengthOfLongestSubstring(std::string str){
    if(str.empty()) return 0;
    std::unordered_set<char> window;
    int left = 0 ; //左标记位
    int res = 0;
    for(int i = 0; i < str.size(); i++){
        while (window.find(str[i]) != window.end()){
            window.erase(str[left]);
            left++;
        }
        res = std::max(res, i-left+1);
        window.insert(str[i]);
    }
    return res;
}
//反转链表
//输入：1->2->3->4->5->NULL
//输出：5->4->3->2->1->NULL
//思路：双指针
ListNode* reverseList2(ListNode* head){
    if(head == nullptr) return nullptr;
    ListNode* curPtr = head;
    ListNode* prePtr = nullptr;
    while(curPtr){
        ListNode* tempPtr = curPtr->p_next_;
        curPtr->p_next_ = prePtr;
        prePtr = curPtr;
        curPtr = tempPtr;
    }
    return prePtr;
}
std::list<int> reverseList3(std::list<int> ls){
    std::list<int> res;
    if(ls.empty()) return res;
    std::reverse(ls.begin(),ls.end());
    return ls;
}
//LRU缓存机制---页面置换算法（最近最少使用的页面淘汰）
//实现：获取get：写入put:当达到上限删除最近最少使用的key。两个操作都变成了O(1)的时间复杂度
//思路：LinkedHashMap，hash+双向链表。哈希表映射双向链表的节点
struct DoubleDirctionList{
    int key;
    int value;
    DoubleDirctionList* prev;
    DoubleDirctionList* next;
    DoubleDirctionList():key(-1),value(-1),prev(nullptr),next(nullptr){}
    DoubleDirctionList(int k, int v):key(k),value(v),prev(nullptr),next(nullptr){}
};
class LRUCache{
public:
    LRUCache(int maxSize):capacity(maxSize){
        head = new DoubleDirctionList();
        tail = new DoubleDirctionList();
        head->next = tail;
        tail->prev = head;
    }
    int getKey(int key){
        if(!cache.count(key)) return -1; //找不到
        DoubleDirctionList* curPtr = cache[key];
        moveNodeToTop(curPtr);
        return curPtr->value;
    }
    void writeKey(int key, int value){
        //不存在对应key
        if(!cache.count(key)){
            //存满了删除
            if(cache.size() == capacity) deleteLastNode();
            //存储 表头插入
            DoubleDirctionList* newNode = new DoubleDirctionList(key,value);
            DoubleDirctionList* tempPtr = head->next;
            head->next = newNode;
            newNode->prev = head;
            newNode->next = tempPtr;
            tempPtr->prev = newNode;
            cache[key] = newNode;
        } else{ //移动到头部 更新value
            DoubleDirctionList* cur = cache[key];
            cur->value = value;
            moveNodeToTop(cur);
        }
    }
private:
    void moveNodeToTop(DoubleDirctionList* cur){
        cur->prev->next = cur->next;
        cur->next->prev = cur->prev;
        DoubleDirctionList* temp = head->next;
        head->next = cur;
        cur->prev = head;
        cur->next = temp;
        temp->prev = cur;
    }
    void deleteLastNode(){
        DoubleDirctionList* last = tail->prev;
        last->prev->next = tail;
        tail->prev = last->prev;
        cache.erase(last->key);
    }
    std::unordered_map<int, DoubleDirctionList*> cache;
    DoubleDirctionList* head;
    DoubleDirctionList* tail;
    int capacity; //LRU最大容量
};
class LRUCache2{
public:
    LRUCache2(int cap):capacity(cap) {}
    int getKey(int key){
        if(!cache.count(key)) return -1;
        auto it  = cache[key];
        mList.splice(mList.end(),mList,it);
        return it->second;
    }
    void put(int key, int value){
        if(cache.count(key)){
            cache[key]->second = value;
            mList.splice(mList.end(),mList,cache[key]);
        } else{
            if(cache.size()==capacity){
                cache.erase(mList.begin()->first);
                mList.erase(mList.begin());
            }
            cache[key] = mList.insert(mList.end(),std::make_pair(key,value));
        }
    }
private:
    std::list<std::pair<int, int>> mList;
    std::unordered_map<int, std::list<std::pair<int,int>>::iterator> cache;
    int capacity; //LRU最大容量
};
//数组中的第K个最大元素
//输入：[3,2,1,5,6,4] k=2
//输出：5
//输入：[3,2,3,1,2,4,5,5,6] k = 4
//输出：4
//思路：堆排序 时间复杂度O(n)
// 递归方式构建大根堆(len是arr的长度，index是第一个非叶子节点的下标)
void adjustBuild(std::vector<int> &arr, int len, int index){
    int left = 2*index + 1; // index的左子节点
    int right = 2*index + 2;// index的右子节点
    int maxIdx = index;
    if(left<len && arr[left] > arr[maxIdx])     maxIdx = left;
    if(right<len && arr[right] > arr[maxIdx])     maxIdx = right;
    if(maxIdx != index){
        std::swap(arr[maxIdx], arr[index]);
        adjustBuild(arr, len, maxIdx);
    }
}
// 堆排序
void heapSort(std::vector<int> &arr, int size){
    // 构建大根堆（从最后一个非叶子节点向上）
    for(int i=size/2 - 1; i >= 0; i--){
        adjustBuild(arr, size, i);
    }
    // 调整大根堆
    for(int i = size - 1; i >= 1; i--){
        std::swap(arr[0], arr[i]);      // 将当前最大的放置到数组末尾
        adjustBuild(arr, i, 0); // 将未完成排序的部分继续进行堆排序
    }
}
//K个一组反转列表
//输入：1->2->3->4 k=2
//输出：2->1->4->3
//思路：递归解决子问题，每次更新子链表的首尾节点
void reverseListList(ListNode* head) {
    ListNode* pre = nullptr, *cur = head, *next;
    while(cur) {
        next = cur->p_next_;
        cur->p_next_ = pre; //核心语句---把当前的节点的next指针指向pre所指的节点
        pre = cur;
        cur = next;
    }
}
ListNode* reverseKGroup(ListNode* head, int k){
    int cnt = 0;
    ListNode* end = head; //end表示待翻转的最后一个节点
    while(end && ++cnt<k) end = end->p_next_; //找到待翻转的最后一个节点，以end表示
    if(count<k) return head; //待翻转节点数不足k个，即不用翻转
    ListNode* temp = end->p_next_; //用temp来保存下一次需要翻转的开始节点
    end->p_next_ = nullptr; //赋值为NULL是为了reverse函数能够识别应该结束的时刻
    reverseListList(head); //开始一次k个节点的翻转
    //因为翻转后，原来的开始节点head变成了结束节点
    head->p_next_ = reverseKGroup(temp, k);//原来的结束节点end变成了开始节点
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
//最大子数组和---最大和的连续数组
//输入：[-2,1,-3,4,-1,2,1,-5,4]
//输出：6 [4,-1,2,1]
//思路：动态规划（只要求得到结果， 不需要知道子数组）
//定义状态：dp[i]表示以nums[i]结尾的连续子数组的最大和
//转移方程：如果dp[i-1]<=0,说明dp[i-1]对dp[i]是负影响，dp[i-1]+nums[i]还不如nums[i]
//初始状态：dp[0]=nums[0]
//返回值：返回最大的dp
int maxSubArray(std::vector<int>& nums){
    int pre = 0;
    int res = nums[0];
    for(int i = 1; i < nums.size(); i++){
        pre = std::max(pre+nums[i],nums[i]);
        res = std::max(res, pre);
    }
    return res;
}
//排序数组
//输入：nums = [5,2,3,1]
//输出：[1,2,3,5]
//思路：快速排序/基准元素
void quickSort(std::vector<int>& nums, int left, int right){
    if(left >= right) return ;
    int i,j,base,temp;
    i = left, j = right;
    base = nums[left]; //取左边为基准
    while(i<j){
        while(nums[j]>=base && i<j) j--;
        while(nums[i]<=base && i<j) i++;
        if(i<j){
            std::swap(nums[i],nums[j]);
        }
    }
    nums[left] = nums[i];
    nums[i] = base;  //已经更换基准数
    quickSort(nums, left, i-1);
    quickSort(nums, i+1, right); //递归处理左右子数组
}
//合并两个有序链表
//输入：1->2->4   1->3->4
//输出：1->1->2->3->4->4
//思路：递归
ListNode* MergeList2(ListNode* headA, ListNode* headB){
    if(headA == nullptr || headB == nullptr) return nullptr;
    ListNode* pMerageHead = nullptr;
    if(headA->m_value_ < headB->m_value_) {
        pMerageHead = headA;
        pMerageHead->p_next_ = MergeList2(headA->p_next_, headB);
    } else{
        pMerageHead=headB;
        pMerageHead->p_next_ = MergeList2(headA, headB->p_next_);
    }
}
//两数之和---返回下标
//输入：[2,7,11,15] k=9
//输出：[0,1] nums[0]+nums[1] = 9
//思路：暴力+哈希
std::vector<int> twoSum2(std::vector<int>& nums, int k){
    if(nums.size()==0) return {};
    for(int i = 0; i<nums.size(); ++i){
        for(int j = i+1; j<nums.size(); ++j){
             int key = k - nums[i];
            if(nums[i]+nums[j] == k) return {i,j};
        }
    }
    return {};
}
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
////05.07
//最长回文字串
//输入：s="babad"
//输出：“bab" "aba"
//输入：s="cbbd"
//输出："bb"
//思路：二维动态规划（双指针也可）
//设置状态：dp[i][j]表示索引j到索引i的子串是否回文
int longestPalindrome3(string s) {
    std::vector<std::vector<bool>> dp(s.size(), std::vector<bool>(s.size()); // i，j表示字符子串s(i到j)，故可以用二维数组表示
    int res = 0;
    for (int i = s.size() - 1; i >= 0; i--) {  // 注意遍历顺序
        for (int j = i; j < s.size(); j++) {
            if (s[i] == s[j]) {
                if (j - i <= 1) { // 情况一 和 情况二
                    result++;
                    dp[i][j] = true;
                } else if (dp[i + 1][j - 1]) { // 情况三
                    result++;
                    dp[i][j] = true;
                }
            }
        }
    }
    return result;
}
std::string longestPalindrome(std::string s) {
    int n = s.size(); // 字符串长度
    int init = 0; // 记录最长回文子串的起始点
    int ml = 1; // 记录最大长度
    std::vector<std::vector<int>> dp(n, std::vector<int> (n)); // 记录是否满足回文
    for (int i = 1; i < n; i ++ )
        for (int j = 0; j < i; j ++ )
            // 若满足回文条件
            if (s[j] == s[i]) {
                // 若长度为2或3，则直接判定为回文子串
                if (i - j == 1 || i - j == 2) dp[j][i] = 1;
                else {
                    if (dp[j + 1][i - 1] == 0) continue;
                    dp[j][i] = 1;
                }
                // 若当前串为回文，并且长度大于最大长度，则更新起始点和最大长度
                if (dp[j][i] == 1 && (i - j + 1) > ml) {
                    ml = i - j + 1;
                    init = j;
                }
            }
    return s.substr(init, ml);
}

//二叉树层序遍历
//输入：二叉树【3，9，20，null,null.15.7]
//输出：[3],[9,20][15,7]
//思路：队列。
void levelOrder(TreeNode* root){
    if(root == nullptr) return;
    std::queue<TreeNode*>q;
    q.push(root);
    while(!q.empty()){
        TreeNode* node = q.front();
        q.pop();
        if(node->pLeft) q.push(node->pLeft);
        if(node->pRight) q.push(node->pRight);
    }
}
//搜索旋转排序数组---nums在下标k进行了旋转
//输入： [4,5,6,7,0,1,2], 可能旋转了 求target=0是否存在 --->O(logn)
//输出：4
//思路：二分法
int searchTarget(std::vector<int>& nums,int target){
    if(nums.size() == 0) return -1;
    int left = 0;
    int right = nums.size()-1;
    while(left <= right){
        int mid = (left+right)/2;
        if(target == nums[mid]) return mid;
        if(nums[left] <= nums[mid]){
            if(target < nums[mid] && target >= nums[left]){
                right = mid - 1;
            } else{
                left = mid + 1;
            }
        } else{
            if(target > nums[mid] && target <= nums[right]){
                left = mid + 1;
            } else{
                right = mid - 1;
            }
        }
    }
    return -1;
}
//买卖股票---买入卖出一次
//输入：[8,9,2,5,4,7,1]
//输出：5（在第三天股价=2时买入，第6天股价=7卖出，利润最大。
//思路：动态规划
//状态：dp[i][j]
int maxProfit(std::vector<int>& prices) {
    int size = prices.size();
    // dp为多行两列的数组
    // 第一列表示股票不持股能达到的最大收益
    // 第二列表示股票持股能保持的最大收益
    std::vector<std::vector<int>> dp(size, std::vector<int>(2, 0));
    // 第一天无法卖出，因为没有持股
    dp[0][0] = 0;
    // 第一天买入股票，进入持股状态，收益为减去当天股价
    dp[0][1] = -prices[0];
    // 动态规划刷新第二天至最后一天的结果
    for(int i = 1; i < size; ++i){
        // 第i天不持股的最大收益由两个情况决定：
        // 一是第i-1天不持股的最大收益，如果该值较大，说明已经完成了较优的交易
        // 二是第i-1天持股的最大收益，在第i天卖出了，进入不持股状态，
        // 此时如果卖出的金额比之前买入的金额大，那么该值为正，后续
        // 除非有更大的买卖差值，否则该值不会刷新
        dp[i][0] = std::max(dp[i - 1][0], dp[i - 1][1] + prices[i]);
        // 第i天持股的最大收益由两个情况决定：
        // 一是第i-1天持股的最大收益，如果该值较大，说明前面已经拿到了较便宜的筹码
        // 二是第i天股价的负值，说明在当天进行了买入
        dp[i][1] = std::max(dp[i - 1][1], -prices[i]);
    }
    // 最后一天不持股能达到的最大收益就是最优结果
    return dp[size - 1][0];
}
//买卖股票2---多次买入卖出
//输入：[7,1,5,3,6,4]
//输出：7（第二天买入，第三天卖出，第四天再买入，第五天卖出）
//思路：动态贪心 发现p[i+1]>p[i]就买入
int maxProfit2(std::vector<int>& prices) {
    int res = 0;
    for(int i = 0; i < prices.size()-1; i++){
        if(prices[i+1] > prices[i]){
            res += prices[i+1]-prices[i];
        }
    }
    return res;
}
//岛屿数量---1为陆地 0为谁
//输入： [1,1,1,1,0]
//      [1,1,0,1,0]
//      [1,1,0,0,0]
//      [0,0,0,0,0]
//输出：1
//思路：深度优先遍历
//四个方向遍历，把与x, y相连的1全部标记为-1
void dfs2(std::vector<std::vector<char>> &grid, int x, int y){
    if(x < 0 || x >= grid.size() || y < 0 || y >= grid[0].size() || grid[x][y] != '1')  return;
    grid[x][y] = -1;
    dfs2(grid, x+1, y);//向下
    dfs2(grid, x, y+1);//向右
    dfs2(grid, x, y-1);//向左
    dfs2(grid, x-1, y);//向上
}
int numIslands(std::vector<std::vector<char>>& grid) {
    int res = 0;
    if(grid.empty())    return res;
    int n = grid.size(), m = grid[0].size();
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < m; ++j){
            if(grid[i][j] == '1'){
                ++res;
                dfs2(grid, i, j);
            }
        }
    }
    return res;
}
//有效括号
//输入：（）  {)
//输出： true  false
//思路：栈
bool isValidParenthesis(std::string& s){
    std::stack<char> st;
    for(char c : s){
        if(st.empty()){
            st.push(c);
        } else if(st.top()=='(' && c ==')'){
            st.pop();
        } else if(st.top()=='[' && c ==']'){
            st.pop();
        } else if(st.top()=='{' && c =='}'){
            st.pop();
        } else{
            st.push(c);
        }
    }
    return st.empty();
}
//环形链表--是否有环
//思路：快慢指针相遇
bool hasCycle(ListNode* head){
    ListNode* fast = head;
    ListNode* slow = head;
    if(head == nullptr) return false;
    while(fast != nullptr && fast->p_next_ != nullptr){
        fast = fast->p_next_->p_next_;
        slow = slow->p_next_;
        if(fast == slow) return true;
    }
    return false;
}
//合并两个有序数组--m,n是元素数目
//输入：[1,2,3,0,0,0] [2,5,6]
//输出：[1,2,2,3,5,6]
void mergeNums(std::vector<int>& nums1, int m, std::vector<int>& nums2, int n) {
    int i = m - 1, j = n - 1, index = m + n - 1;
    while (i >= 0 && j >= 0){
        nums1[index--] = nums1[i] > nums2[j] ? nums1[i--] : nums2[j--];
    }
    while (j >= 0) nums1[index--] = nums2[j--];
}
//全排列
//[1,2,3]
//思路：递归回溯
std::vector<std::vector<int>> dfs3(std::vector<int>& nums, int x){
    std::vector<std::vector<int>> res;
    if(x == nums.size()-1) res.emplace_back(nums);
    for(int i = x; x < nums.size(); i++){
        std::swap(nums[i], nums[x]);
        dfs3(nums, x+1);
        std::swap(nums[i], nums[x]);
    }
    return res;
}
//二叉树最近公共祖先---找到该树中两个指定节点的最近公共祖先，可以是本身
//思路：递归
TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
    if(root == nullptr) return nullptr;
    if(root->mValue == p->mValue || root->mValue == q->mValue) return root;
    TreeNode* leftN = lowestCommonAncestor(root->pLeft,p,q);
    TreeNode* rightN = lowestCommonAncestor(root->pRight,p,q);
    if(leftN != nullptr && rightN != nullptr) return root;
    return leftN != nullptr ? leftN : rightN;
}
////05.08
//二叉树锯齿形层次遍历-先左后右，再右后左
//输入：二叉树[3，9，20，null,null.15.7]
//输出：[3],[20,9][15,7]
//思路：队列
std::vector<std::vector<int>> zigzagLevelOrder(TreeNode* root) {
    auto ret = std::vector<std::vector<int>>();
    if (root == nullptr) return ret;
    auto queue = std::vector<const TreeNode*>{root};
    auto isOdd = true;
    while (!queue.empty()) {
        auto sz = queue.size();
        auto level = std::vector<int>();
        for (auto i = 0; i < sz; ++i) {
            auto n = queue.front();
            queue.erase(queue.begin());
            if (isOdd) level.push_back(n->mValue);
            else level.insert(level.begin(), n->mValue);
            if (n->pLeft != nullptr) queue.push_back(n->pLeft);
            if (n->pRight != nullptr) queue.push_back(n->pRight);
        }
        isOdd = !isOdd;
        ret.push_back(level);
    }
    return ret;
}
//反转链表2--反转指定位置
//输入：[1,2,3,4,5] left=2 right=4
//输出：[1,4,3,2,5]
ListNode* reverseBetween(ListNode* head, int left, int right){
    ListNode* dummy = new ListNode(-1);
    dummy->p_next_ = head;
    ListNode* prePtr = dummy;
    for(int i = 0; i < left-1; ++i){
        prePtr = prePtr->p_next_;
    }
    ListNode* curPtr = prePtr->p_next_;
    for(int i = 0; i < right-left; ++i){
        ListNode* tempPtr = curPtr->p_next_;
        curPtr->p_next_ = prePtr;
        prePtr = curPtr;
        curPtr = tempPtr;
    }
    return head;
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

//合并K个升序链表
//输入：list=[[1,4,5],[1,3,4][2,6]]
//输出：[1,1,2,3,4,4,5,6]
//思路：堆
struct Compare {
    bool operator() (ListNode* a, ListNode* b) {
        return a->m_value_ > b->m_value_;
    }
};
ListNode* mergeKLists(std::vector<ListNode*>& lists) {
    // 用最小堆，找到最小的节点，加入结果链表中，C++ 里是优先队列实现的
    std::priority_queue<ListNode*, std::vector<ListNode*>, Compare> pq;
    // 将每个链表的头节点加入最小堆
    for (ListNode* list : lists) {
        if (list) {
            pq.push(list);
        }
    }
    ListNode dummy(0);
    ListNode* tail = &dummy;
    while (!pq.empty()) {
        ListNode* minNode = pq.top();
        pq.pop();
        tail->p_next_ = minNode;
        tail = tail->p_next_;
        if (minNode->p_next_) {
            pq.push(minNode->p_next_);
        }
    }
    return dummy.p_next_;
}
//相交链表
//输入  4->1->8->4->5
//   5->0->1\
//输出：相交为 8
//思路：双指针+一个走完 走下个头 A+B = B+A
ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
    ListNode* p = headA;
    ListNode* q = headB;
    while (p != q){
        if(p) p = p->p_next_;
        else p = headB;
        if(q) q = q->p_next_;
        else q = headA;
    }
    return p;
}
//最长上升子序列
//输入：[10,9,2,5,3,7,101,18]
//输出：4 [2,3,7,101]
//思路：动态规划
int lengthOfLIS(std::vector<int>& nums) {
    int maxres=1;
    int n=nums.size();
    if(n==0) return 0;
    std::vector<int> res(n,1);
    for(int i=1;i<n;i++){
        for(int j=0;j<i;j++){
            if(nums[i]>nums[j]){
                res[i]=std::max(res[i],res[j]+1);
            }
        }
        maxres=std::max(maxres,res[i]);
    }
    return maxres;
}
//字符串相加
//输入：'123' '11'
//输出：‘134’
//思路：
std::string addStrings(std::string num1, std::string num2) {
    int i = num1.length()-1, j = num2.length()-1;
    int carry = 0;
    std::string res = "";
    while(i >= 0 || j >= 0){
        int n1 = 0, n2 = 0;
        if(i >= 0){
            n1 = num1[i]-'0';
        }
        if(j >= 0){
            n2 = num2[j]-'0';
        }
        int sum = n1 + n2 + carry;
        res = std::to_string(sum % 10) + res;
        carry = sum / 10;
        i--;
        j--;
    }
    if(carry > 0){
        res = std::to_string(carry) + res;
    }
    return res;
}
//接雨水
//输入: [0,1,0,2,1,0,1,3,2,1,2,1]
//输出: 6
//思路：单调栈
int trap(std::vector<int>& height) {
    int n = height.size(), ans = 0;
    std::stack<int> st;
    for (int i = 0; i < n; i++) {
        while (!st.empty() && height[st.top()] <= height[i]) {
            int top = st.top();
            st.pop();
            if (st.empty()) break;
            ans += (i - st.top() - 1)
                    * (std::min(height[st.top()], height[i]) - height[top]);
        }
        st.push(i);
    }
    return ans;
}
//重排链表
//输入：1->2->3->4->5
//输出：1->5->2->->4->3
//思路：找到中间节点，后半段反向；双指针从1和，n开始扫，后半段插入前半段
void reorderList(ListNode* head){
    int n = 0; //统计链表的节点个数
    for(ListNode *p = head; p; p = p->p_next_) n++;
    if(n <= 2) return;
    ListNode* curPtr = head;
    //找中间节点 走到
    for(int i = 0; i<(n+1)/2 -1;i++) curPtr = curPtr->p_next_;
    ListNode* a = curPtr; //指向中间了
    ListNode* b = curPtr->p_next_;
    //将链表的后半段反向
    while(b){
        ListNode *p_next_ = b-> p_next_;
        b->p_next_  = a;//保留b的next节点
        a = b, b = p_next_;
    }
    curPtr->p_next_ = 0; //给前半段链表添加一个结束标记
    curPtr->p_next_ = 0; //给前半段链表添加一个结束标记
    ListNode *tmp = nullptr;
    //一个是遇到中点停止，一个是head, a左右两指针相遇停止
    while(head && head != a){
        tmp = a->p_next_;
        a->p_next_ = head->p_next_;
        head->p_next_ = a;
        head = head->p_next_->p_next_;
        a = tmp;
    }
}
//环形链表2---返回入环的第一个节点
//思路：双指针
ListNode *detectCycle(ListNode *head) {
    if (head == nullptr || head->p_next_ == nullptr)
        return nullptr;
    if (head->p_next_ == head)
        return head;
    ListNode* node1 = head->p_next_;
    ListNode* node2 = head->p_next_->p_next_;
    while (node1 != node2) {//判断链表是否存在环，同第141题
        if (node2 == nullptr || node2->p_next_ == nullptr)
            return nullptr;
        node2 = node2->p_next_->p_next_;
        node1 = node1->p_next_;
    }
    node2 = head;
    while (node1 != node2) {//由前述数学分析，双指针再次指向的同一节点，即为环的入口节点
        node1 = node1->p_next_;
        node2 = node2->p_next_;
    }
    return node1;
}
//合并区间
//输入: [[1,3],[2,6],[8,10],[15,18]]
//输出: [[1,6],[8,10],[15,18]]
//解释: 区间 [1,3] 和 [2,6] 重叠, 将它们合并为 [1,6].
//思路：先排序 按照左端点，然后进一步判断
std::vector<std::vector<int>> mergeSection(std::vector<std::vector<int>>& nums){
    std::vector<std::vector<int>> res;
    if(nums.empty()) return res;
    std::sort(nums.begin(),nums.end());
    int left = nums[0][0], right = nums[0][1];
    for(int i = 1; i < nums.size(); i++){
        if (nums[i][0] > right){
            res.push_back({left,right});
            left = nums[i][0], right = nums[i][1];
        } else{
            right = nums[i][0], right[i][1];
        }
    }
    res.push_back({left, right});
    return res;
}
//二叉树最大路径和
//输入：[1,2,3] ---> 6
//输入：[-10,9,20,null,null,15,7] --->42 [15->20->7]
int maxPath(TreeNode* node, int& ans)
{
    if(node == nullptr) return 0;
    int left = maxPath(node->pLeft, ans);
    int right = maxPath(node->pRight, ans);
    //经过根节点的情况下，最大路径和
    int lm = node->mValue + std::max(left, 0) + std::max(right, 0);
    //不经过根节点情况下，最大路径和为当前节点，加上左右子树中的最大值
    int rt = node->mValue + std::max(0, std::max(right, left));
    ans = std::max(ans, std::max(lm, rt));
    return rt;
}
int maxPathSum(TreeNode* root) {
    int ans = INT_MIN;
    maxPath(root, ans);
    return ans;
}
//删除链表的倒数第N个节点
//输入 1->2->3->4->5 n=2
//输出 1->2->3->5
//思路 双指针
ListNode *removeNthFromEnd(ListNode *head, int n) {
    ListNode *dummyNode = new ListNode();
    dummyNode->p_next_ = head;
    ListNode *fast = dummyNode;
    ListNode *slow = dummyNode;
    //fast先走n步
    while (n-- && fast != nullptr) {
        fast = fast->p_next_;
    }
    fast = fast->p_next_;  //fast 走n+1 步
    //fast slow 一起向前移动,直到fast为空
    while (fast != nullptr) {
        fast = fast->p_next_;
        slow = slow->p_next_;
    }
    //删除slow的下一个结点
    slow->p_next_ = slow->p_next_->p_next_;
    return dummyNode->p_next_;
}

//复原IP地址--IP四个部分
//输入：2552551135“
//输出：'255.255.11.135' '255.255.111.35'
//思路：回溯的方法
//最多四步走，cnt<=4
//走到末尾了，合法ip
//子串数字不能大于255
//开头不能为0
bool isValidSection(const std::string& section){
    int len = section.size();
    if(len == 0 || len >3) return false;
    if(len > 1 && section[0] == '0') return false;
    int n = 0;
    for(int i = 0; i < len; i++){
        char c = section[i];
        if(c < '0' || c >'9') return false;
        n = n*10 + (c-'0');
        if(n>255) return false;
    }
    return true;
}
void backtrack(const std::string& s, int left, 
                int sectionNum, std::string& ip, std::vector<std::string>& res){
    if(sectionNum == 4){
        if(left == s.size()){
            res.emplace_back(ip);
        }
        return;
    }
    for (int i = left; i < left + 3; i++) {
            if (i >= s.size()) {
                break;
            }
            string section = s.substr(startIndex, i - startIndex + 1);
            if (isValidSection(section)) {
                int len = ip.length();
                if (sectionNum > 0) {
                    ip += ".";
                }
                ip += section;
                backtrack(s, i + 1, sectionNum + 1, ip, result);
                ip.resize(len);
            }
        }
}
std::vector<std::string> restoreIpAddresses(std::string s) {
    std::vector<string> result;
    std::string ip;
    backtrack(s, 0, 0, ip, result);
    return result;
}
//最长公共子序列
//输入：text1 = "abcde", text2 = "ace" 
//输出：3 （ace)
//思路：二维动态规划问题
int longestCommonSubsequence(std::string text1, std::string text2) {
        int m = text1.length(), n = text2.length();
        std::vector<std::vector<int>> dp(m + 1, std::vector<int>(n + 1));
        for (int i = 1; i <= m; i++) {
            char c1 = text1.at(i - 1);
            for (int j = 1; j <= n; j++) {
                char c2 = text2.at(j - 1);
                if (c1 == c2) {
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                } else {
                    dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
                }
            }
        }
        return dp[m][n];
}
//二叉树中序遍历--左右中
//输入：root = [1,null,2,3]
//输出：[1,3,2]
void inorder(TreeNode* root, vector<int>& res) {
    if (!root) {
        return;
    }
    inorder(root->left, res);
    res.push_back(root->val);
    inorder(root->right, res);
}
std::vector<int> inorderTraversal(TreeNode* root) {
    std::vector<int> res;
    inorder(root, res);
    return res;
}
//删除链表重复元素
//输入：head = [1,2,3,3,4,4,5]
//输出：[1,2,5]
//思路：hash存储次数
ListNode* deleteDuplicate(ListNode* head){
    std::unordered_map<int,int> cnt;
    if(head == nullptr) return nullptr;
    ++cnt[head->val];
    head->p_next_  = deleteDuplicate(head->p_next_);
    if(cnt[head->m_value] >= 2) return head->p_next_;
    return head;
}
//二分查找法
//输入: nums = [-1,0,3,5,9,12], target = 9
//输出: 4
//解释: 9 出现在 nums 中并且下标为 4
int twoPartSort(std::vector<int>& nums, int target){
    int i = 0;
    int j = nums.size()-1;
    while(i<=j){
        int m = (i+j)/2;
        if(nums[m]<target) i = m+1;
        else if (nums[m]>target) j = m-1;
        else return m;
    }
    return -1;
}
//二叉树的右视图
//输入: [1,2,3,null,5,null,4]
//输出: [1,3,4]
//思路：BFS
std::vector<int> rightSideView(TreeNode* root) {
        std::queue<TreeNode*> nodeQueue;
        if(root != nullptr) nodeQueue.push(root);
        std::vector<int> res;
        while (!nodeQueue.empty()) {
            for(int i = 0; i < nodeQueue.size(); i++){
                TreeNode* node = que.front();
                que.pop();
                if(i = nodeQueue.size()-1) res.push_back(node->mValue);
                if(node->pLeft) que.push(node->pLeft);
                if(node->pRight) que.push(node->pRight);
            }
        }
        return res;
}

////05.09
//无重复子串 --- 利用ordered_set利用元素无序且唯一的特性
//反转链表 --- 一定保存curNode的next 后继变前继 前继成cur cur成后继
//LRU缓存 --- ordered_set作为cache 双向链表（拥有前继和后继指针）注意移动表头的操作 std::list的操作
//数组K最大元素 --- 堆排序 大根堆 根结点大于左右节点
//             构建大根堆需要最后非叶节点 size/2 -1 交换首尾 末尾元素继续构建
//K个一组反转链表 --- 找个范围递归 然后 反转
//三数和为0 --- 排序 选定一个基准num[i] left right

//最大子数组和 ---  一维动态规划 dp[i] 比较num[i]结尾的值 和 num[i-1]之前子数组的最大
//最长回文子串 --- 二维动态规划 dp[i][j] 表示索引i到索引j的子串是否回文 s[i] == s[j] dp[i][j] = dp[i+1][j-1]
//           --- 一个字符串的头尾两个字符都不相等，那么这个字符串一定不是回文串；递进判断子串
//           --- 一个字符串的头尾两个字符都相等：情况一：下标i = j 情况二：i-j=1 相差1 情况三：相差大于1
//           --- 注意遍历顺序从下到上，从左到右
//for (int i = s.size() - 1; i >= 0; i--) {  // 注意遍历顺序
//    for (int j = i; j < s.size(); j++) {
//        if (s[i] == s[j]) {
//            if (j - i <= 1) { // 情况一 和 情况二
//                result++;
//                dp[i][j] = true;
//            } else if (dp[i + 1][j - 1]) { // 情况三
//                result++;
//                dp[i][j] = true;
//            }
//        }
//    }
//}
//买卖股票 --- 二维动态规划 五部曲
//        --- 确定dp数组的含义
//        --- 确定递推公式
//        --- 初始化数组
//        --- 遍历顺序
//最长上升子序列 --- dp[i]以nums[i]结尾的最长递增子序列的长度
//             --- if (nums[i] > nums[j]) dp[i] = max(dp[i], dp[j] + 1);
//for (int i = 1; i < nums.size(); i++) {
//    for (int j = 0; j < i; j++) {
//        if (nums[i] > nums[j]) dp[i] = max(dp[i], dp[j] + 1);
//    }
//    if (dp[i] > result) result = dp[i]; // 取长的子序列
//}

//快排---找基准 然后递归左右数组即可
//合并两个有序链表---递归找头
//两数之和 --- 利用hash

//二叉树层序遍历 --- 利用队列
//二叉树 锯齿形遍历 --- 队列 标志位控制左右方向

//搜索旋转数组 --- 二分

//岛屿数量 --- DFS
//有效括号 ---  栈 后进先出
//环形链表 --- 快慢指针相遇
//合并有序数组 --- 直接做
//全排列 -- 递归
//二叉树最近公共祖先 --- 递归

//螺旋矩阵 硬解
//合并K个升序链表 ---优先级队列
//接雨水 ---  栈

////05.10
//寻找两个正序数组的中位数
//输入：nums1 = [1,2], nums2 = [3,4]
//输出：2.50000
//解释：合并数组 = [1,2,3,4] ，中位数 (2 + 3) / 2 = 2.5
//思路：双指针
double findMedianSortedArrays(std::vector<int>& nums1, std::vector<int>& nums2) {
    int m = nums1.size();
    int n = nums2.size();
    int total = m + n;
    int half = total / 2; //计算中位数所在的位置
    int i = 0, j = 0;
    int prev = 0, curr = 0; //初始化两个变量用于记录当前元素和前一个元素
    while (i + j <= half) {
        prev = curr;
        if (i < m && (j >= n || nums1[i] < nums2[j])) {
            curr = nums1[i];
            i++;
        } else {
            curr = nums2[j];
            j++;
        }
    }
    if (total % 2 == 1) {
        return curr;
    } else {
        return (prev + curr) / 2.0;
    }
}
//两个栈实现队列 队列尾部插入节点 队列头部删除节点
template <typename T>
class CQueue{
public:
    CQueue();
    ~CQueue();
    void addTail(const T& node);
    T deleHead();
private:
    std::stack<T> stack1;
    std::stack<T> stack2;
};
template<typename T>void CQueue<T>::addTail(const T &node) {
    stack1.push(node);
}
template<typename T>T CQueue<T>::deleHead(){
    if(stack2.size() <= 0){
        while(stack1.size() > 0){
            T& data = stack1.top();
            stack2.push(data);
        }
    }
    if(stack2.size() == 0) throw new std::exception;
    T head = stack2.pop();
    stack2.pop();
    return  head;
}
//排序链表
//输入：4->2->1->3
//输出：1->2->3->4
//思路：归并排序
ListNode* mergeSortList(ListNode* head){
    if (!head || !head->next) return head;
    int n = 0;
    for(auto i = head; i != nullptr; i = i->next)  ++n; //n为链表长度
    auto dummy = new ListNode(-1);
    dummy->next = head;
    // 总体循环次数为logn
    for(int i = 1; i < n; i += i){
        auto beg = dummy;
        // 最开始1个和1个合并为两个，然后2,2->4, 4,4->8
        for(int j = 0; j + i < n; j += 2*i){ //j += 2*i为下一块的起点
            auto left = beg->next;
            auto right = left;
            for(int k = 0; k < i; ++k)  right = right->next;//right指向第二块的起点,每块有i个节点，越过i个节点即可
            // merge第一块和第二块，起点分别为left, right
            // 第一块的节点数为i, 第二块的节点数可能小于i（为i-1）,因为节点个数有奇偶之分,所以需要检查right != NULL
            int p = 0, q = 0;//计数
            while(p < i && q < i && right != NULL){
                if(left->val <= right->val){
                    beg->next = left;
                    left = left->next;
                    ++p;
                }else{
                    beg->next = right;
                    right = right->next;
                    ++q;
                }
                beg = beg->next;
            }
            while(p < i){// 可能还有剩余未处理的
                beg->next = left;
                beg = beg->next;
                left = left->next;
                ++p;
            }
            while(q < i && right != NULL){
                beg->next = right;
                beg = beg->next;
                right = right->next; //right会指向下一块的起点
                ++q;
            }
            // 处理完之后beg指向的是两块中（已经排序好）元素最大的那个节点
            beg->next = right; //调整beg位置，将beg和下一块连接起来
        }
    }
    return dummy->next;
}
//下一个排列---给出下一个更大的 否则最小的开始
//输入：3,2,1
//输出：1，2，3
//思路：全排列 后面遍历
void nextPermutation(vector<int>& nums) {
    int n = nums.size(), i = n - 2, j = n - 1;
    while (i >= 0 && nums[i] >= nums[i + 1]) --i;
    if (i >= 0) {
        while (nums[j] <= nums[i]) --j;
        swap(nums[i], nums[j]);
    }
    reverse(nums.begin() + i + 1, nums.end());
}
//x的平方根
//二分
int mySqrt(int x) {
    if (x <= 1) return x;
    int left = 0, right = x;
    while (left < right) {
        int mid = left + (right - left) / 2;
        if (x / mid >= mid) left = mid + 1;
        else right = mid;
    }
    return right - 1;
}
//字符串转换为整数
//输入：“42”
//输出：42
//输入: "words and 987"
//输出: 0
//解释: 第一个非空字符是 'w', 但它不是数字或正、负号。
//因此无法执行有效的转换。
int myAtoi(string str) {
    if (str.empty()) return 0;
    int sign = 1, base = 0, i = 0, n = str.size();
    while (i < n && str[i] == ' ') ++i;
    if (i < n && (str[i] == '+' || str[i] == '-')) {
        sign = (str[i++] == '+') ? 1 : -1;
    }
    while (i < n && str[i] >= '0' && str[i] <= '9') {
        if (base > INT_MAX / 10 || (base == INT_MAX / 10 && str[i] - '0' > 7)) {
            return (sign == 1) ? INT_MAX : INT_MIN;
        }
        base = 10 * base + (str[i++] - '0');
    }
    return base * sign;
}
//爬楼梯
//输入：3
//输出：3  3种方法 1+1+1 1+2 2+1
//思路：动态规划
int climbStairs(int n) {
    std::vector<int> dp(n+1); //初始化数组
    if (n<=2){
        return n;
    }
    dp[0] = 0;
    dp[1] = 1;
    dp[2] = 2;
    for(int i=3;i<n+1;i++){
        dp[i] = dp[i-2] + dp[i-1];

    }
    return dp[n];
}
//两数相加--给定两个非空链表来表示两个非负整数，位数按照逆序方式存储
//输入：(2 -> 4 -> 3) + (5 -> 6 -> 4)
//输出：7 -> 0 -> 8
//原因：342 + 465 = 807
ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
    ListNode *pHead = new ListNode(0);  //头节点
    ListNode* p = pHead;
    ListNode *p1 = l1,*p2 = l2,*p3 = pHead;
    ListNode *pNext = nullptr;
    int sum = 0,temp = 0;
    while(p1 != nullptr || p2 != nullptr){
        int x = (p1 != nullptr) ? p1->m_value_ : 0;
        int y = (p2 != nullptr) ? p2->m_value_ : 0;
        sum = x + y + temp;
        temp = sum / 10;    //保存十位数字
        p3->next = new ListNode (sum % 10);        //创建个位数字节点添加到链表中
        p3 = p3->p_next_;
        if(p1 != nullptr)
            p1 = p1->p_next_;
        if(p2 != nullptr)
            p2 = p2->p_next_;
    }
    if(temp == 1){
        p3->next = new ListNode(1);
    }
    return pHead->p_next_;
}
//括号生成
//输入：n = 3
// 输出：[
//“((()))”,
//“(()())”,
//“(())()”,
//“()(())”,
//“()()()”
//]
//思路：动态规划
std::vector<std::string> generateParenthesis(int n) {
    std::vector<std::vector<std::string>> alls(1, { "" }); // 初始化带有f(0)的存储数组
    // 从n = 1开始计算f(n)，并存储到alls中
    for (int j = 1; j <= n; j++) {
        std::vector<std::string> temp;
        for (int i = 0; i < j; i++)
            for (std::string& left : alls[i])
                for (std::string& right : alls[j - 1 - i])
                    temp.push_back("(" + left + ")" + right); // "("f(n-1)")"f(0), "("f(n-2)")"f(1) ... "("f(1)")"f(n-2), "("f(0)")"f(n-1)
        alls.push_back(temp);
    }
    return alls[n];
}
//比较版本号
int compareVersion(string v1, string v2) {
    int i = 0, j = 0;
    while(i < v1.size() || j < v2.size())
    {
        int num1 = 0, num2 = 0;
        while(i < v1.size() && v1[i] != '.') num1 = num1 * 10 + v1[i++] - '0';
        while(j < v2.size() && v2[j] != '.') num2 = num2 * 10 + v2[j++] - '0';
        if(num1 > num2) return 1;
        else if( num1 < num2) return -1;
        i++,j++;
    }
    return 0;
}
//缺失的第一个正数
//输入：[1,2,0]
//输出：3
//思路：
int firstMissingPositive(vector<int>& nums) {
    for(int i=0;i<nums.size();i++)
    {
        if(nums[i]<=0 || nums[i]>nums.size())
        {
            nums[i]=0;
        }
    }
    int n=nums.size();
    for(int i=0;i<nums.size();i++)
    {
        if(nums[i]%(n+1)!=0)
        {
            int t=0;
            t=nums[i]%(n+1);
            nums[t-1]+=n+1;
        }
    }
    int a=1,j=0;        
    for(;j<nums.size();j++)
    {
        if(nums[j]<(n+1))
        {
            // cout<<"aa"<<endl;
            a=j+1;
            break;
        }
    }
    return (j==n)?n+1:a;
}
