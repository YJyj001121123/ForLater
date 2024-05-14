/***
 * 链表
 * 链表是一种通过指针串联在一起的线性结构，每一个节点由两部分组成
 * 一个是数据域一个是指针域（存放指向下一个节点的指针），最后一个节点的指针域指向null（空指针的意思）
 * 双链表
 * 每一个节点有两个指针域，一个指向下一个节点，一个指向上一个节点
 * 既可以向前查询也可以向后查询
 * 循环链表
 * 
 * 链表不是连续分布在内存
 * 删除节点增加O(1)
 * 查询O(n)
 * 
*/
// 单链表
#include <iostream>
struct ListNode {
    int val;  // 节点上存储的元素
    ListNode *next;  // 指向下一个节点的指针
    ListNode(int x) : val(x), next(nullptr) {}  // 节点的构造函数
};
//移除链表元素
//输入：head = [1,2,6,3,4,5,6], val = 6 
//输出：[1,2,3,4,5]
//思路：考虑头节点
ListNode* removeElement(ListNode* head, int val){
    while(head != nullptr && head->val == val){
        ListNode* tmp = head;
        head = head->next;
        delete tmp;
    }
    // 删除非头结点
    ListNode* cur = head;
    while (cur != nullptr && cur->next!= nullptr) {
        if (cur->next->val == val) {
            ListNode* tmp = cur->next;
            cur->next = cur->next->next;
            delete tmp;
        } else {
            cur = cur->next;
        }
    }
    return head;
}
//设计链表--get\addAthead\addAtTail\deleteAtIndex
class MyLinkedList {
public:
    // 定义链表节点结构体
    struct LinkedNode {
        int val;
        LinkedNode* next;
        LinkedNode(int val):val(val), next(nullptr){}
    };

    // 初始化链表
    MyLinkedList() {
        _dummyHead = new LinkedNode(0); // 这里定义的头结点 是一个虚拟头结点，而不是真正的链表头结点
        _size = 0;
    }

    // 获取到第index个节点数值，如果index是非法数值直接返回-1， 注意index是从0开始的，第0个节点就是头结点
    int get(int index) {
        if (index > (_size - 1) || index < 0) {
            return -1;
        }
        LinkedNode* cur = _dummyHead->next;
        while(index--){ // 如果--index 就会陷入死循环
            cur = cur->next;
        }
        return cur->val;
    }

    // 在链表最前面插入一个节点，插入完成后，新插入的节点为链表的新的头结点
    void addAtHead(int val) {
        LinkedNode* newNode = new LinkedNode(val);
        newNode->next = _dummyHead->next;
        _dummyHead->next = newNode;
        _size++;
    }

    // 在链表最后面添加一个节点
    void addAtTail(int val) {
        LinkedNode* newNode = new LinkedNode(val);
        LinkedNode* cur = _dummyHead;
        while(cur->next != nullptr){
            cur = cur->next;
        }
        cur->next = newNode;
        _size++;
    }

    // 在第index个节点之前插入一个新节点，例如index为0，那么新插入的节点为链表的新头节点。
    // 如果index 等于链表的长度，则说明是新插入的节点为链表的尾结点
    // 如果index大于链表的长度，则返回空
    // 如果index小于0，则在头部插入节点
    void addAtIndex(int index, int val) {
        if(index > _size) return;
        if(index < 0) index = 0;        
        LinkedNode* newNode = new LinkedNode(val);
        LinkedNode* cur = _dummyHead;
        while(index--) {
            cur = cur->next;
        }
        newNode->next = cur->next;
        cur->next = newNode;
        _size++;
    }

    // 删除第index个节点，如果index 大于等于链表的长度，直接return，注意index是从0开始的
    void deleteAtIndex(int index) {
        if (index >= _size || index < 0) {
            return;
        }
        LinkedNode* cur = _dummyHead;
        while(index--) {
            cur = cur ->next;
        }
        LinkedNode* tmp = cur->next;
        cur->next = cur->next->next;
        delete tmp;
        //delete命令指示释放了tmp指针原本所指的那部分内存，
        //被delete后的指针tmp的值（地址）并非就是NULL，而是随机值。也就是被delete后，
        //如果不再加上一句tmp=nullptr,tmp会成为乱指的野指针
        //如果之后的程序不小心使用了tmp，会指向难以预想的内存空间
        tmp=nullptr;
        _size--;
    }

    // 打印链表
    void printLinkedList() {
        LinkedNode* cur = _dummyHead;
        while (cur->next != nullptr) {
            std::cout << cur->next->val << " ";
            cur = cur->next;
        }
        std::cout << std::endl;
    }
private:
    int _size;
    LinkedNode* _dummyHead;

};
//反转链表
//输入：1->2->3->4->5->NULL
//输出：5->4->3->2->1->NULL
//思路：双指针
ListNode* reverseList2(ListNode* head){
    if(head == nullptr) return nullptr;
    ListNode* curPtr = head;
    ListNode* prePtr = nullptr;
    while(curPtr){
        ListNode* tempPtr = curPtr->next;
        curPtr->next = prePtr;
        prePtr = curPtr;
        curPtr = tempPtr;
    }
    return prePtr;
}
//两两交换链表节点--交换相邻的
//输入：[1,2,3,4]
//输出：[2,1,4,3]
//思路：模拟行为
ListNode* swapNode(ListNode* head){
    ListNode* dummyHead = new ListNode(0); // 设置一个虚拟头结点
    dummyHead->next = head;
    ListNode* cur = dummyHead;
    while(cur->next!=nullptr&&cur->next->next!=nullptr){
        ListNode* temp = cur->next;
        ListNode* temp2 = cur->next->next->next;
        cur->next = cur->next->next;
        cur->next->next = temp;
        cur->next->next->next = temp2;
        cur = cur->next->next; // cur移动两位，准备下一轮交换
    }
    ListNode* result = dummyHead->next;
    delete dummyHead;
    return result;
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
//相交链表
//输入  4->1->8->4->5
//   5->0->1\
//输出：相交为 8
//思路：双指针+一个走完 走下个头 A+B = B+A
ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
    ListNode* p = headA;
    ListNode* q = headB;
    while (p != q){
        if(p) p = p->next;
        else p = headB;
        if(q) q = q->next;
        else q = headA;
    }
    return p;
}
//环形链表--是否有环
//思路：快慢指针相遇
bool hasCycle(ListNode* head){
    ListNode* fast = head;
    ListNode* slow = head;
    if(head == nullptr) return false;
    while(fast != nullptr && fast->next != nullptr){
        fast = fast->next->next;
        slow = slow->next;
        if(fast == slow) return true;
    }
    return false;
}
//环形链表2---返回入环的第一个节点
//思路：双指针
ListNode *detectCycle(ListNode *head) {
    if (head == nullptr || head->next == nullptr)
        return nullptr;
    if (head->next == head)
        return head;
    ListNode* node1 = head->next;
    ListNode* node2 = head->next->next;
    while (node1 != node2) {//判断链表是否存在环，同第141题
        if (node2 == nullptr || node2->next == nullptr)
            return nullptr;
        node2 = node2->next->next;
        node1 = node1->next;
    }
    node2 = head;
    while (node1 != node2) {//由前述数学分析，双指针再次指向的同一节点，即为环的入口节点
        node1 = node1->next;
        node2 = node2->next;
    }
    return node1;
}