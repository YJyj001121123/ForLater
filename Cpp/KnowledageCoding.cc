// #include "pch.h" // Use with pre-compiled header
#include <thread>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <list>
#include <stack>

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
    ListNode* low = head,
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





