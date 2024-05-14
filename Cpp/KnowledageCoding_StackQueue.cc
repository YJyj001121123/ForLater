
#include <stack>
/***
 * 栈---先进后出
 * 栈是数据结构 不是容器 没有iterator
 * container adapter（容器适配器）
 * 栈的底层实现可以是vector，deque，list 都是可以的， 主要就是数组和链表的底层实现
 * 默认是dequeue
 * 
 * 队列---先进先出
 * list 为起底层实现
*/

//栈实现队列
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
//队列实现栈
class MyStack {
public:
    std::queue<int> que1;
    std::queue<int> que2; // 辅助队列，用来备份
    /** Initialize your data structure here. */
    MyStack() {

    }

    /** Push element x onto stack. */
    void push(int x) {
        que1.push(x);
    }

    /** Removes the element on top of the stack and returns that element. */
    int pop() {
        int size = que1.size();
        size--;
        while (size--) { // 将que1 导入que2，但要留下最后一个元素
            que2.push(que1.front());
            que1.pop();
        }

        int result = que1.front(); // 留下的最后一个元素就是要返回的值
        que1.pop();
        que1 = que2;            // 再将que2赋值给que1
        while (!que2.empty()) { // 清空que2
            que2.pop();
        }
        return result;
    }

    /** Get the top element. */
    int top() {
        return que1.back();
    }

    /** Returns whether the stack is empty. */
    bool empty() {
        return que1.empty();
    }
};