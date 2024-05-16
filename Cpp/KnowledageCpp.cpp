#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <thread>
#include <list>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <future>
#include <pthread.h>
#include <stack>
#include <queue>


////引用和指针 & *  ｜｜ 值传递、引用传递、指针传递
//指针本身是实体是需要内存空间的，引用只是别名
//指针可以重新赋值，指向不同的变量；引用初始化后不能更改的
//不能多级引用但可以多级指针
//值传递：实参的值传递形参，函数内形参修改不会影响到实参
//引用传递：实参的引用传递，函数内对形参的修改会影响到实参
//指针传递：实参的地址传递给形参，函数内对形参的修改会影响到实参
//在栈上，指针和引用都位于局部变量空间，但是指针存放的是指针的地址，引用存放的变量的地址
void swap_value(int a, int b){
    std::swap(a,b);
}
void swap_quote(int &a, int &b){
    std::swap(a,b);
}
void swap_pointer(int *a, int *b){
    std::swap(a,b);
}
int quote_execute(){
    int x = 10;
    int y = 20;
    swap_value(x, y);
    std::cout<<x<<y; //x=10,y=20
    swap_quote(x, y);
    std::cout<<x<<y; //x=20,y=10
    swap_pointer(&x, &y);
    std::cout<<x<<y; //x=20,y=10
    return 0;
}

////static
//静态变量初始化只有一次，编译器为其分配内存。
//静态局部变量：函数内部声明的。即使函数结束，值也会保留。C中，代码前完成初始化（分配好内存，就会初始化）。
// C++ 初始化在执行时候进行初始化，必须执行构造和析构函数。
void static_conut(){
    static int cnt = 0;
    cnt++;
    std::cout<<cnt; //多次调用cnt累加，不会每次从0开始
}
//静态全局变量：函数外部声明。只局限在声明它的文件内 extern其他文件可以访问
static int cnt = 0; //当前文件共享的变量，其他文件无法访问
//静态成员变量：类中，类的所有对象共有的。
//静态成员函数：类的所有对象共享。只能访问静态成员数据、其他静态成员函数和类外部函数。
class StaticClass{
public:
    static int cnt;
    static void increaseCounter(){
        cnt++;
    }
    void decrease(){
        cnt--;
    }
};
int StaticClass::cnt = 0; //需要在类外初始化
void static_member(){
    std::shared_ptr<StaticClass> s1 = std::make_shared<StaticClass>();
    StaticClass s2; //s1 s2共享cnt
    StaticClass::increaseCounter();//不用创建对象，直接调用静态成员函数
}
//回调函数情况：类成员函数需要回调函数，而这个回调函数需要全局访问。需要static
// 或者传递它的上下文信息（this）调用非静态
class Foo{
    static void fun1(){
        std::cout<<"123";
    }
    void fun2(){
        std::cout<<"123";
    }
    void startThread(){
        std::thread t(fun1);
        t.detach();
    }
    void startThread2(){
        std::thread t(&Foo::fun2, this);
        t.detach();
    }
};

////const
//const常量限定符，通知编译器不可修改。
//const修饰基本数据类型
const int const_a1 = 10;
int const const_a2 = 10;  //等价
//const修饰指针变量*和引用变量&
const int *const_a3; //常量指针：修饰指针所指向的变量，const_a3不能变
int* const const_a4 = nullptr ; // 指针常量：指针本身为常量，所指向内容可变
const int &const_a5 = 1;
int const &const_a6 = 1; //等价
//const修饰函数
const int const_fun2(){};
class ConstClass{
private:
    int i ;
    const int size = 10; //错误不能在类声明初始化const数据成员！！！
    const int size2;  //正确的 在构造函数的初始化列表中完成、不能在构造中赋值
                    //或者static const int 去类外声明
public:
    ConstClass(int a) : size2(a){}
    void const_fun1(const int& a){ } // 保证了原本的内容a不变化
    int get() const{
        return i;
    }  //不对i进行修改，防止函数体对i修改
    const int& get2(){
        return i;
    } //不对返回值进行修改，防止函数体对i修改
};
const ConstClass concla(1); //const修饰类对象，不能改变任何成员变量，调用非const成员函数

////mutable
//只能作用在类的非静态和非常量数据。少用

////extern
//具有外部链接的属性。三种链接属性：none\external\internal
extern "C"  int extern_fun1(); //声明extern_fun1使用C链接

//// strcat\strcpy\strncpy\memset\memcpy
//strcat C库函数
//strcpy src的'\0\结束的字符串复制到dest。src和dest内存区域不能重叠.如果stc.length()>dest.length() 栈溢出，异常。可以dest后面置为NULL停止复制。
char *strcpy_v(char *dest, char *src);
//strncpy 将字符串src中最多n个字符复制到字符数组dest中
char* strncpy_v(char* dest, char* src, size_t n);
//memcpy 一般是内存的复制，复制内容没有限制。
void *memcpy_v(void *dest, const void *src, size_t count);
//memset s前n个字节替换为ch
void *memset_v(void *s , int ch , size_t n);
//string转int std::stoi/stol/stoll
//int转string std::to_string

////深浅拷贝
//未定义显示拷贝构造函数，系统默认调用拷贝函数（浅拷贝）。简单的赋值
//浅拷贝：没有指针可行。如果有指针，两个指针指向同一个地址，析构两次会出现悬挂指针。
//深拷贝：会在堆内存申请空间存储。

////模版
//泛型编程 先编译一次模版，再去编译带参数具体的函数或类
//函数模版：使用时候参数化 可以隐式或者显式实例化
template<typename T1, typename T2, typename T3>
T1 sum(T2 v2, T3 v3) {
    return static_cast<T1>(v2 + v3);
}
auto ret = sum<long>(1L, 23); //指定T1, T2和T3交由编译器来推断
template<typename T1, typename T2, typename T3>
T3 sum_alternative(T1 v1, T2 v2) {
    return static_cast<T1>(v1 + v2);
}
//auto ret = sum_alternative<long>(1L, 23); //error，只能从左向右逐一指定
auto ret2 = sum_alternative<long,int,long>(1L,23); //ok, 谁叫你把最后一个T3作为返回类型的呢？
//函数模版的重载
template<typename T>
void templateFun1(T& t){}
template<typename T>
void templateFun1(T* t){}
void templateFun1(std::string s){} //普通函数
//类模板
//不能隐式实例化
template<typename T>
class TemplateClass{
public:
    explicit TemplateClass(const T& params):t(params){}
    template<typename U>
    void add(const U&u);
private:
    T t;
};
template<typename T>
template<typename U>
void TemplateClass<T>::add(const U& u) {
    std::cout << t + u << std::endl;
}
void template_execute(){
    TemplateClass<int> p(42);
    p.add(1.1); //自动推断U为double，打印出43.1
}

////struct
//C中：用户自定义数据类型 无权限不可以成员存在函数
//C++：抽象数据类型，可以有函数，默认public权限

////成员初始化列表
class initList {
    initList(int x, int y);
    int m_x;
    int m_y; //声明的顺序
};
initList::initList(int x, int y) : m_y(y), m_x(m_y)
{
} //先初始化m_x然后m_y
//按照声明的顺序初始化，而不是出现在初始化列表的顺序

////构造函数不能虚函数，析构可以虚函数
//构造函数不能虚函数
//虚函数需要虚表的指针vptr。构造函数还没有实例化，没有内存空间的。
//没有意义：虚函数的作用在于通过父类的指针或者引用来调用它的时候可以变成调用子类的那个成员函数。而构造函数是在创建对象时自己主动调用的，不可能通过父类的指针或者引用去调用，因此也就规定构造函数不能是虚函数
//析构可以虚函数
//防止内存泄漏，确保派生类销毁
//析构函数无参数，无返回值，不能重载。
//- 构造函数：
//第一步：虚拟基类的构造函数
//第二步：基类的构造函数
//第三步：类类型的成员对象的构造函数
//第四步：派生类的构造函数
//- 析构函数
//相反

////类实现静态分配和动态分配
//一个进程的内存空间而言，可以在逻辑上分成3个部份：代码区，静态数据区和动态数据区
//静态分配：
//发生在程序编译和链接的时候
//栈可以静态分配：编译器完成（局部变量的分配
//重载new delete设置为private
//将类的new操作符设置为private或者delet
//动态分配：
//程序调入和执行的时间
//堆动态分配的
//栈可以动态分配：alloca()函数实现。
//构造析构置为protected 派生类去动态创建
//类的析构函数设置为private

////合成默认构造函数和合成复制构造函数
//移动构造&& 右值引用，源对象的资源指针或资源句柄复制给目标对象，避免深拷贝。
//合成默认构造函数
//带有默认构造函数的类成员对象：如果一个类没有任何构造函数，但它含有一个成员对象，而后者有默认构造函数，那么编译器就为该类合成出一个默认构造函数。
//带有默认构造函数的基类
//带有虚函数的类、虚基类
//合成的默认构造函数中，只有基类子对象和成员类对象会被初始化。所有其他的非静态数据成员都不会被初始化。
//合成复制构造函数 &
//复制构造函数有且只有一个本类类型对象的引用形参，通常使用 const限定。因为复制构造函数只是复制对象，没有必要改变传递来的对象的值。
//每个类必须有一个复制构造函数。如果类没有定义复制构造函数，编译器自动合成一个。

////类组合
class one{

};
class two{
protected:
    one o_;
};

////四种转换
//static_cast：（静态转换）类型间和指针间转换，派生类-->基类是安全的。反之不安全（没有动态类型检查）。
//dymatic_cast：（动态转换）基类-->派生类。基类必须有虚函数（运行时类型信息保存在虚表）。
//const_cast：常量到非常量的转换。
//reinterpret_cast：可以类型转指针。指针转类型。

////vector和list
//vector：
//数组。连续的内存空间，能够高效随机查找，时间复杂度O(1)，插入删除O(n)
//当vector内存不够，重新申请double的内存，复制过去
//迭代器使用后就失效
//list：
//双向链表内存空间不是连续的，指针操作。随机查找，时间复杂度O(n)，插入删除O(1)。只能遍历访问。
//迭代器使用后可以继续使用
void Vector_fun(){
    //初始化
    std::vector<int> vector1;
    std::vector<int> vector2(5,2);
    std::vector<int> vector3(1);
    std::vector<int> vector4(vector1.begin()+2, vector1.end());
    std::vector<int> vector5 = {1,2,3,4,5,6,7};
    //
    vector2.assign(vector5.begin(),vector5.begin()+3); //5的值给2 0，1，2三个元素
    vector1.assign(4,2); //赋值
    vector5.back(); //返回5最后一个元素
    vector5.front(); //返回5的第一个元素
    vector5[2]; //取第三个元素
    vector2.clear();  //清空元素  此时vector2仍旧占空间的
    std::vector<int>().swap(vector2);
    auto res = vector5.empty(); //5是否空
    vector5.push_back(1); //最后插入元素首先会创建这个元素，然后再将这个元素拷贝或者移动到容器中（如果是拷贝的话，事后会自行销毁先前创建的这个元素）
    vector5.emplace_back(1); //效率更好；在实现时，则是直接在容器尾部创建这个元素，省去了拷贝或移动元素的过程。
    vector5.pop_back(); //删除最后一个元素
    vector5.erase(vector5.begin()+1, vector5.end()-3); //删除指定范围元素
    vector5.insert(vector5.begin(), 5); //指定位置插入
    vector5.emplace(vector5.begin(),2);
    vector5.insert(vector5.begin()+1,vector2.begin(),vector2.end());//指定位置插入2 一个范围
    vector5.size(); //当前元素个数
    vector5.resize(100); //补充100个元素 多删少补
    vector5.resize(20,2); // 补充少补2
    vector5.reserve(200); //容量扩充到200
    vector5.swap(vector2); //交换
    std::sort(vector5.begin(), vector5.end()); //排序
    std::reverse(vector5.begin(), vector5.end()); //倒置
    std::copy(vector5.begin(), vector5.end(),vector2.begin()); //复制
    auto find_ = std::find(vector5.begin(), vector5.end(),1); //找1元素返回位置迭代器
}
void List_func(){
    //初始化
    std::list<int> list1;
    std::list<int> list2(12);
    std::list<int> list3(12,2);
    std::list<int> list4(list2);
    std::list<int> list5(list2.begin(),++list2.end());
    //
    list2.size(); //当前大小
    list2.max_size(); //最大容量
    list2.resize(16); //更改大小
    auto r = list2.empty(); //判空
    list2.push_back(2); //末尾添加元素
    list2.pop_back();
    list2.emplace_back(2);
    list2.push_front(2); //头部添加元素
    list2.pop_front();
    list2.emplace_front(2);
    list2.emplace(list2.begin(),2);//任意位置插入2
    list2.insert(list2.begin(),2);
    list2.insert(list2.begin(),list3.begin(),list3.end());
    list2.clear();//清空元素
    list2.front(); // 访问第一个元素
    list2.back(); //访问末尾元素
    list2.erase(list2.begin()); //任意位置删除元素
    list2.assign(list3.begin(),list3.begin()); //3的值给2 0，1，2三个元素
    list2.assign(4,2); //赋值
    list2.merge(list4); //合并升序
    list2.splice(list2.begin(),list3); //指定位置合并
    list2.unique();//删除重复元素
}

//// set\unordered_set\map\unordered_map
//set：
// 元素会自动排序，不允许存在重复的，底层是红黑树，键值就是实值
//插入删除时间复杂度O(logn)
//unordered_set：
//无序的，底层hash表。
//map:
//自动根据各键值对的键的大小，按照既定的规则进行排序，底层红黑树
//插入删除时间复杂度O(logn)
//unordered_map：
//无序的，底层hash表
void SetFun(){
    std::set<int> set1;
//    std::set<int> set2(10); 错误的
    std::set<int> set2(set1);
    std::set<int> set3(set1.begin(),set1.end());
    //
    set1.size(); //当前大小
    set1.max_size(); //最大容量
    auto r = set1.empty(); //判断空
    auto f = set1.count(2); //查找个数 一般0或1 存在为1 不存在为0
//    set1.emplace(set2.begin(),2);//任意位置插入2 错误
    set1.insert(set2.begin(),2);
    set1.emplace(2);
    set1.erase(set1.begin());
    set1.erase(set1.begin(),set1.end());
    set1.clear();
    auto z = set1.find(2);

    std::unordered_set<int> unset1;
    std::unordered_set<int> unset2{1,2,3,4};
    std::unordered_set<int> unset3(16); //16个bucket
    unset1.insert(2);
    unset1.insert(unset2.begin(),unset2.end());
    unset1.emplace(2); //插入
    auto z2 = unset1.find(2);
    auto res = unset1.count(2); //查找2 有几个  一般0或者1
    unset1.bucket_count();//返回桶数量
    unset1.bucket_size(2);//返回桶2的数量
    unset1.size(); //返回总元素
    unset1.bucket(2); //找到2 在哪个bucket
    unset1.max_size(); // 最大容纳元素
    auto u = unset1.empty();
    unset1.clear();
    unset1.erase(2);
}
void MapFunction(){
    std::map<int, std::string> map1;
    map1.size();
    auto a = map1.empty();
    auto b = map1.find(2); //返回迭代器
    map1.at(2); //乍到对应的值
    map1.insert(std::pair<int,std::string>(1,"2"));
    map1.emplace(std::pair<int,std::string>(1,"2"));
    map1.emplace(1,"22");
    map1.insert(std::make_pair(1,"2"));
    map1.erase(2);
    map1.clear();
    auto c = map1.count(2); //寻找 一般0或者1
}
////stack 和 queue
//stack
//后进先出，顶部弹出
//queue
//先进先出
//只能访问第一个和最后一个元素的容器适配器
//只能在容器的末尾添加新元素，只能从头部移除元素。
void stackFunc(){
    std::stack<int> stack1_; //默认底层容器为queue
    std::stack<int,std::list<int>> stack2_; //底层容器为双向链表
    std::stack<int,std::list<int>> stack3_(stack2_);
    stack1_.push(3);//入栈
    stack1_.emplace(2);
    stack1_.pop();//出栈
    stack1_.top();//访问栈顶
    stack1_.size();
    auto res = stack1_.empty();
}
void queueFunc(){
    std::queue<int> data;
    std::list<int> data_{0,1,2,3};
    std::queue<int,std::list<int>> q(data_);
    data.emplace(2);
    data.push(2);
    data.pop(); //删除头部
    data.size();
    data.empty();
    data.front(); //返回头部元素
    data.back();
}

////迭代器有什么用处？泛型算法的参数为什么接收的都是迭代器？
//迭代器iterator是C++ STL的组件之一，作用是用来遍历容器，
//而且是通用的遍历容器元素的方式，无论容器是基于什么数据结构实现的，
//尽管不同的数据结构，遍历元素的方式不一样，但是用迭代器遍历不同容器的代码是完全一样的。
//不同容器的迭代器是不能进行比较的
//容器的元素进行增加、删除操作后，原来的迭代器就全部失效了

////函数指针
//函数指针
//指向函数（首地址）的指针变量，本质是指针变量
//一个指向函数的指针，可以调用函数
/* 求最大值，返回值是int类型，返回两个整数中较大的一个*/
int funcPtrMax(int a, int b) {
    return a > b ? a : b;
}
/* 求最小值，返回值是int类型，返回两个整数中较小的一个*/
int funcPtrMin(int a, int b) {
    return a < b ? a : b;
}
int(*f)(int, int); // 声明函数指针，指向返回值类型为int，有两个参数类型都是int的函数
void funcPtr(){
    f = funcPtrMax; // 函数指针f指向求最大值的函数max(将max函数的首地址赋给指针f)
    int c = (*f)(1, 2);
    printf("The max value is %d \n", c);
    f = funcPtrMin; // 函数指针f指向求最小值的函数min(将min函数的首地址赋给指针f)
    c = (*f)(1, 2);
    printf("The min value is %d \n", c);
}

////内存分配
//stack 栈
//编译器管理，存放函数从参数，局部变量。向下生长，内存地址减少方向
//heap 堆
//程序员操作。 向上生长，内存地址增加方向
//static 全局取 静态区
//全局变量静态变量存放data。未初始化的全局变量和未初始化的静态变量在相邻的另一块区域bss。程序结束后由系统释放。
//文字常量区
//程序代码区

////智能指针
//shared_ptr：引用计数实现 智能指针不能互相引用 多线程环境加锁
//shared_ptr的引用计数本身是安全且无锁的，但对象的读写则不是：
// shared_ptr 有两个数据成员，一个是指向的对象的指针，还有一个是引用计数管理对象
// 当智能指针发生拷贝的时候，标准库的实现是先拷贝智能指针，再拷贝引用计数对象（拷贝引用计数对象的时候，会使use_count加一）
// 这两个操作并不是原子操作，隐患就出现在这里。
// 两个线程中智能指针的引用计数同时++或--，这个操作不是原子的，
// 假设引用计数原来是1，++了两次，可能还是2，这样引用计数就错乱了，违背了原子性
//手写shared_ptr：
class SharedCount {
public:
    SharedCount() : count_{1} {}
    void add() { ++count_; }
    void minus() { --count_; }
    int get() const { return count_; }
private:
    std::atomic<int> count_; //atomic线程安全的访问修改共享数据
};
template<typename T>
class SharedPtr{
public:
    //构造函数创建SharedCount
    SharedPtr(T *ptr) : ptr_(ptr), ref_cnt_(new SharedCount){}
    SharedPtr() : ptr_(nullptr), ref_cnt_(new SharedCount){}
    ~SharedPtr(){clear();}
    //拷贝构造和拷贝赋值时候 计数加一
    SharedPtr(const SharedPtr& p){
        this->ptr_ = p.ptr_;
        this->ref_cnt_ = p.ref_cnt_;
        ref_cnt_->add();
    }
    SharedPtr& operator=(const SharedPtr& p) {
        clear();
        this->ptr_ = p.ptr_;
        this->ref_cnt_ = p.ref_cnt_;
        ref_cnt_->add();
        return *this;
    }
    //移动构造和移动赋值 引用计数不变，清空原参数的指针
    SharedPtr(SharedPtr&& p) {
        this->ptr_ = p.ptr_;
        this->ref_cnt_ = p.ref_cnt_;
        p.ptr_ = nullptr;
        p.ref_cnt_ = nullptr;
    }
    SharedPtr& operator=(SharedPtr&& p)  noexcept {
        clear();
        this->ptr_ = p.ptr_;
        this->ref_cnt_ = p.ref_cnt_;
        p.ptr_ = nullptr;
        p.ref_cnt_ = nullptr;
        return *this;
    }

private:
    void clear(){
        if(ref_cnt_){
            ref_cnt_->minus();  //创建出来的SharedPtr引用计数是1
            if(ref_cnt_->get() == 0){
                if(ptr_) delete ptr_;
                delete ref_cnt_;
            }
        }
    }
    SharedCount *ref_cnt_; //引用计数 记录当前有多少个shared_ptr共享同一个对象
    T* ptr_; //指向对象的指针
};
//手写unique_ptr
//不允许拷贝，禁止调用拷贝构造函数和拷贝赋值函数
template<typename T>
class UniquePtr{
public:
    UniquePtr(T* ptr) : ptr_(ptr){}
    UniquePtr() : ptr_(nullptr){}
    UniquePtr(const UniquePtr& p) = delete;
    UniquePtr& operator=(const UniquePtr& p) = delete;

    UniquePtr(UniquePtr&& p) {
        this->ptr_ = p.ptr_;
        p.ptr_ = nullptr;
    }
    UniquePtr& operator=(UniquePtr&& p) {
        clear();
        this->ptr_ = p.ptr_;
        p.ptr_ = nullptr;
        return *this;
    }
    T* get() const { return ptr_; }
    T* operator->() const { return ptr_; }
    T& operator*() const { return *ptr_; }
    operator bool() const { return ptr_; }
    ~UniquePtr() { clear(); }
private:
    void clear() {
        if (ptr_) delete ptr_;
    }
    T* ptr_;
};
//weak_ptr
template<typename T>
class WeakPtr{
public:
    WeakPtr(const SharedPtr<T>& s):ptr(s.ptr_){}
    T& operator*(){
        return *ptr;
    }
    T* operator->(){
        return ptr;
    }
private:
    T* ptr;
};

////内存对齐
//不是所有的硬件平台都能访问任意地址上的任意数据的；
//某些硬件平台只能在某些地址处取某些特定类型的数据，否则抛出硬件异常
//性能考虑：数据结构栈尽可能的在自然边界对齐，为了访问未对齐的内存，处理器需要两次访问。

////浮点数判断
//不能直接==，要与设定精度比较
//BOOL : if ( !a ) or if(a)
//int : if ( a == 0)
//float : const EXPRESSION EXP = 0.000001
//if ( a < EXP && a >-EXP)
//pointer : if ( a != nullptr) or if(a == nullptr)

////lambda函数
// []是捕获列表
// 默认捕获是&和=。
//[ ]。空捕获列表，lambda不能使用所在函数中的变量。
//[=]。函数体内可以使用Lambda所在作用范围内所有可见的局部变量（包括Lambda所在类的this），并且是值传递方式（相当于编译器自动为我们按值传递了所有局部变量）。
//[&]。函数体内可以使用Lambda所在作用范围内所有可见的局部变量（包括Lambda所在类的this），并且是引用传递方式（相当于编译器自动为我们按引用传递了所有局部变量）。
//[this]。函数体内可以使用Lambda所在类中的成员变量。
//[a]。将a按值进行传递。按值进行传递时，函数体内不能修改传递进来的a的拷贝，因为默认情况下函数是const的。要修改传递进来的a的拷贝，可以添加mutable修饰符。
//[&a]。将a按引用进行传递。
//[=，&a, &b]。除a和b按引用进行传递外，其他参数都按值进行传递。
//[&, a, b]。除a和b按值进行传递外，其他参数都按引用进行传递。
// （）形参列表
//修饰符 mutable 允许函数体修改各个复制捕获的对象，以及调用非const对象
// 默认情况下，值拷贝的变量，lambada不会改变，需要修改就加mutable
// ->返回类型

////迭代器 ++it 和 it++
//前置返回引用，后置返回对象，前置不会产生临时对象，后置必须产生临时对象。

////成员函数调用delete this
//当调用delete this时，类对象的内存空间被释放。在delete this之后进行的其他任何函数调用，
//只要不涉及到this指针的内容，都能够正常运行。
//析构函数如果调用delete this，无限递归，堆栈溢出。delete本质是调用析构。

////volatile
//一种类型修饰符，用它声明的类型变量表示可以被某些编译器未知的因素更改，比如：操作系统、硬件或者其它线程等。遇到这个关键字声明的变量，
// 编译器对访问该变量的代码就不再进行优化，从而可以提供对特殊地址的稳定访问

////函数调用
//__stdcall是函数恢复堆栈，
// 只有在函数代码的结尾出现一次恢复堆栈的代码；在编译时就规定了参数个数，
// 无法实现不定个数的参数调用；
// __cdecl
//__cdecl是调用者恢复堆栈，假设有100个函数调用函数a，
// 那么内存中就有100端恢复堆栈的代码；可以不定参数个数；
// 每一个调用它的函数都包含清空堆栈的代码，
// 所以产生的可执行文件大小会比调用__stacall函数大。
//寄存器：
//rax：函数返回值使用。
//rsp：栈指针寄存器，指向栈顶。
//栈：栈顶和栈底。地址是最低到高的。栈顶是最低地址。
//sp（栈指针）。ebp指向栈底，esp指向栈顶。
//简化理解：main函数（caller）调用foo函数(callee）。、
// esp被foo使用来指示栈顶，ebp相当于基准指针。
// 从main传递到foo的参数以及foo本身的局部变量都可以使用ebp参考，
// 加上偏移量找到。传递的参数，最后一个参数先压入栈，第一个参数位于栈顶的。

/***
 * ESP---> ｜                                ｜
 *         ｜被调用者保存的寄存器现场 EBX,ESI和EDI｜
 *         ｜临时空间                          ｜
 *         ｜局部变量#2                        ｜    EBP-8
 *         ｜局部变量#1                        ｜    EBP-4
 * EBP---> ｜调用者的ESP                       ｜
 *         ｜返回地址                          ｜
 *         ｜实际参数#1                        ｜    EBP+8
 *         ｜实际参数#2                        ｜    EBP+12
 *         ｜实际参数#3                        ｜    EBP+16
 *         ｜调用者保存的寄存器现场 EAX，ECX和EDX ｜
 */

/***
 * ESP---> ｜实际参数#1                        ｜
 *         ｜实际参数#2                        ｜
 *         ｜实际参数#3                        ｜
 *         ｜调用者保存的寄存器现场 EAX，ECX和EDX ｜
 *         ｜           ...                   ｜
 * EBP---> ｜                                 ｜
  */

/***
* a = foo(12,15,18)
调用者
push dword 18 //压入参数
push dword 15
push dword 12
call foo    //main call指令调用
当call执行时，eip指令指针寄存器的内容压入栈 调用完成后，esp指向上图的返回地址

Callee foo取得程序控制权，必须完成：建立自己的栈帧，为局部变量分配空间，保存其他寄存器的值。
ebp的值必须保留，ebp进栈，esp内容赋值给ebp
push ebp
mov ebp, esp
分配局部变量
sub esp, 20 //分配20字节
整个foo执行中，esp可能会上下移动，但ebp不动的

返回控制权到main，foo返回值保存在eax寄存器
消除栈帧
mov esp,ebp
pop ebp
(leave
ret);

控制权回到调用者main，传递到参数不需要了，需要弹出栈
add esp 12
完整来看
long test(int a, int b){
    a = a + 3;
    b = b + 5;
    return a + b;
}

int main(){
    printf("%d", test(10,90));
    return 0;
}

int main(){
00401070    push ebp      //ebp入栈，保存现场
00401071    mov  ebp,esp
00401073    sub  esp,40h   //函数使用的堆栈（16*4bytes）
00401076    push ebx
00401077    push esi
00401078    push edi
00401079    lea  edi,[ebp-40h].
0040107C    mov  ecx,10h
00401081    mov  eax,0CCCCCCCCh
00401086    rep stos   dword ptr [edi].//初始化该函数栈空间为0CCCCCCCC
    printf("%d", test(10,90));
00401088    push 5Ah   //参数入栈
0040108A    push 0Ah
0040108C    call @ILT+0(test) (00401005) //调用 转向eip 00401005 跳到被调函数
00401091    add  esp,8   //清除栈
00401094    push eax
00401095    push offset string "%d" (0042201c)
0040109A    call printf (004010d0)
0040109F    add  esp,8
    return 0;
004010A2   xor  eax,eax. //eax清零
*/

//MFC消息机制
//消息：使用统一的结构体（MSG）存放信息
/***
 * typedef struct tagMsg{
    HWND hwnd; //句柄
    UINT message//标明消息的类型
    WPARAM wParam;
    LPARAM lParam; //两个最灵活的变量，为不同的消息类型时，存放数据含义不一样。
    DWORD time; //产生消息的时间
    POINT pt; //产生消息时鼠标的位置
} MSG
 */
//消息分类：
//系统定义消息：窗口消息（WM_CREATE、WM_MOUSEMOVE）(与窗口运作相关的、创建窗口、绘制窗口、销毁窗口）、命令消息（WM_COMMAND）（用户从菜单选中一个命令项目，按下快捷键或者控件，发送命令消息）、控件通知（WM_COMMAND、WM_NOTIFY扩展COMMAND消息）。
//程序定义消息：WM_USER。
//队列消息：消息先保存在消息队列，消息循环从此队列取消息分发到窗口处理。FIFO的方式。
//消息ID范围：
//系统定义范围[0x0000,0x03ff] (WM_NULL(0x0000)空消息）
//程序定义消息:RegisterWindowMessage[0xC000,0xFFFF]【用来和其他应用程序通信，为了ID的唯一性，使用::RegisterWindowMessage来得到该范围的消息ID 】
//消息系统分级：
//第一级：Windows内核的系统消息队列（按照线程不同分发到下一级）
//第二级：App的UI线程消息队列（每一个UI线程都有自己的消息循环，发送给窗体对象）
//第三级：处理消息的窗体对象（每一个对象使用窗体过程函数（WindowProc）处理消息。
//其中WM_COMMAND命令消息会统一由当前活动的窗口接收，绕行后被其他CCmdTarget对象处理。WM_COMMAND控件消息会反射通知子窗口，如果处理了，停止。反之通过OnCmdMsg继续发送。
/***
 * CCmdTarget 继承 CObject
 * CWinThread\ CWnd \CDocument 继承 CCmdTarget
 * CWinApp继承CWinThread
 * CObjec类：基类，完成动态空间分配与回收。
CCmdTarget类：实现消息映射的基类。负责消息发送、等待、派遣的内容。
CWinThread类：创建处理消息循环。MFC有且仅有一个CWinApp。
CWnd类：通用窗口类。onwndmsg
SendMessage和PostMessage：
前者直接发送给窗口过程处理，处理完返回；后者投递到消息队列，立即返回。
GetMessage和PeekMessage：
前者有消息且消息不为WM_QUIT，返回true；有消息且消息为WM_QUIT，false；没有消息的话，挂起UI线程，控制权反给系统。后者不会阻塞，有消息就true，可以删除队列消息。
MFC消息映射：消息分派机制。
避免产生宏大的虚函数分配表。
*/

////回调函数
//- 函数F1调用函数F2时候，F1通过参数给F2传递了另外一个函数F3的指针。在F2执行的过程中，F2调用F3。（callback）F3被称为回调函数。
//可以解耦。回调中，主程序把回调函数像参数一样传入主函数，通过修改参数（callback）就可以实现不同功能。
//- 还有一种用法std::function和std::bind的使用，std::function实例可以对任何可以调用的目标实体进行存储、复制、调用。（目标包括普通函数、Lambda表达式、函数指针、其他对象）。std::bind绑定函数。
int callback(){
    printf("Hello");
    return 0;
}
void callback2(int x){
    printf("Hello, this is Callback_1: x = %d ", x);
}
int Handle(int (*Callback)()){
    printf("h1");
    Callback();
    printf("h2");
}
void Handle2(void (*Callback)(int)){
    printf("h1");
    Callback(2);
    printf("h2");
}
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
void testBind(){
    TestB b;
    TestA a;
    b.FB1(std::bind(&TestA::FA2, &a));
}
//std::function通常是一个函数对象类，可以包装其他任意的函数对象，可以有参数，返回一个右值类型。闭包类型可以隐式转换为std::function。
//std::bind预先把指定可调用实体的某些参数绑定到已有变量，产生一个新的可调用实体，不论普通函数还是函数对象，成员函数都可以绑定。

////随机数
//随机数本身是伪随机数：根据一个数（种子）为基准推算的一系列数字，符合正态分布。计算机开机后，种子的值是确定的。
//-rand()：范围0～RADN_MAX，每次执行时是相同的。int rand(void);
//-srand()：设置rand()产生随机数的种子，一般会采用当前时钟。void srand(unsigned int seed)；

////禁止栈产生对象
//protected析构函数， private禁用了继承
//=delete 禁止使用编译器默认生成的函数

////final
//禁用继承

////std::call_once 初始化代码使用一次
//Singleton mode
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

////malloc\brk\mmap
//每个进程都有独立的虚拟内存空间，虚拟地址页表查询获得真实物理地址
//linux虚拟空间内存：由低到高-->
// 只读段(只读不写，#define、C常量字符串等）
// 数据段（全局、静态变量空间）
// 堆（动态内存，malloc\new 大部分来源与此，栈顶通过函数brk和sbrk动态调整）
// %esp 执行栈顶，往低地址方向变化；brk/sbrk 函数控制堆顶_edata往高地址方向变化。
// 文件映射区域 （动态库，共享内存等映射物理空间的内存，一般mmap函数分配的）
// 栈（维护函数调用的上下文空间，一般为8M，ulimit-s查看）
// 内核虚拟空间（用户不可见，内核管理的）
//malloc 维护一个内存空闲链表，申请内存时，搜索链接，找到空闲空间分配。如果搜索不到，使用brk指针去申请
//进程两种分配内存方式：brk和mmap
//brk指针：数据段data的最高地址指针_edata往搞地址推。
//mmap：在进程虚拟空间地址空间中（堆和栈中间，称为文件映射区域的地方）找一块空闲虚拟内存。

////std::thread
void inc(std::atomic<int> *p){
    std::mutex mu;
    for(int i = 0 ; i < 100; i++){
//        std::unique_lock<std::mutex> lc(mu); //确保加锁
//        std::lock_guard<std::mutex>lc(mu);
        mu.lock();
        (*p)++;
        mu.unlock();
    }
}

void testThread(){
    std::atomic<int> a = 0;
    std::thread ta(inc,&a);
    std::thread tb(inc,&a);
    ta.join();
    tb.detach();
}

////std::condition_variable
//条件变量，视为pthread_cond_t的封装
//使用条件变量可以让一个线程等待其他线程的通知（wait\wait_for\wait_until)
//也可以给其他线程发送通知（notify_one\notify_all)
//必须配合锁使用，在等待时候因为有解锁和重新加锁，必须手动操作加解锁的锁，lock_guard就不行了，unique_lock可以的
std::mutex mutex_;
std::condition_variable cv_;
void ConDI(){
    std::thread** t = new std::thread*[10];
    for(int i = 0; i <10; i++){
        t[i] = new std::thread([](int index){
            std::unique_lock<std::mutex> lc(mutex_);
            cv_.wait_for(lc, std::chrono::hours(1000));
        },i );

        std::this_thread::sleep_for(std::chrono::microseconds(50));
    }
    for(int i = 0; i < 10; i++){
        std::lock_guard<std::mutex> _(mutex_);
        cv_.notify_one();
    }

    for(int i = 0; i < 10; i++){
        t[i]->join();
        delete t[i];
    }
    delete t;
}

////std::promise/future
//用来在线程之间进行简单的数据交互，而不需要考虑锁的问题，
// 线程 A 将数据保存在一个 promise 变量中，
// 另外一个线程 B 可以通过这个 promise 变量的 get_future() 获取其值，
// 当线程 A 尚未在 promise 变量中赋值时，线程 B 也可以等待这个 promise 变量的赋值
//一个 future 变量只能调用一次 get()，如果需要多次调用 get()，
// 可以使用 shared_future，通过 promise/future 还可以在线程之间传递异常。
std::promise<std::string > val;
void ProFur(){
    std::thread ta([](){
        std::future<std::string > fu = val.get_future();
    });
    std::thread tb([](){
        std::this_thread::sleep_for(std::chrono::microseconds(50));
        val.set_value("pro is set");
    });
    ta.join();
    tb.join();
}

std::mutex mutex2;
////std::packaged_task
void packTask(){
    auto run = [=](int index){
        {
            std::lock_guard<std::mutex> lc(mutex2);
            std::cout << "task" << index;
        }
        std::this_thread::sleep_for(std::chrono::microseconds(50));
        return index *1000;
    };

    std::packaged_task<int(int)> ptr1(run);
    std::packaged_task<int(int)> ptr2(run);
    std::thread ta([&](){
        ptr1(2);
    });
    std::thread tb([&](){
        ptr1(3);
    });
    int f1 = ptr1.get_future().get();
    int f2 = ptr2.get_future().get();
    std::cout<<f1;
    std::cout<<f2;
    ta.join();
    tb.join();
}

///pthread
//支持posix系统
//pthread_create 创建线程
//pthread_create(&thread, &attr, f, static_cast<void *>(&args));
//其中f是函数，args是所有参数打包成的结构体。因为pthread_create的第四个参数类型是void*，所以需要强制转型 pthread_create只接受void *f(void *)这样的函数签名
//pthread_exit 终止线程
//
//pthread_join 连接线程
//
//pthread_detach 分离线程
//pthread_mutex_lock(&mutex)

// 注意pthread_*函数返回的异常值
//pthread_mutex_t mutex;
//pthread_cond_t condvar;
//
//std::queue<int> msgQueue;
//struct Produce_range {
//    int start;
//    int end;
//};
//
//void *producer(void *args)
//{
//    int start = static_cast<Produce_range *>(args)->start;
//    int end = static_cast<Produce_range *>(args)->end;
//    for (int x = start; x < end; x++) {
//        usleep(200 * 1000);
//        pthread_mutex_lock(&mutex);
//        msgQueue.push(x);
//        pthread_mutex_unlock(&mutex);
//        pthread_cond_signal(&condvar);
//        printf("Produce message %d\n", x);
//    }
//    pthread_exit((void *)0);
//    return NULL;
//}
//
//void *consumer(void *args)
//{
//    int demand = *static_cast<int *>(args);
//    while (true) {
//        pthread_mutex_lock(&mutex);
//        while (msgQueue.size() <= 0) {
//            pthread_cond_wait(&condvar, &mutex);
//        }
//        if (msgQueue.size() > 0) {
//            printf("Consume message %d\n", msgQueue.front());
//            msgQueue.pop();
//            --demand;
//        }
//        pthread_mutex_unlock(&mutex);
//        if (!demand) break;
//    }
//    pthread_exit((void *)0);
//    return NULL;
//}
//
//
//int main()
//{
//    pthread_attr_t attr;
//    pthread_attr_init(&attr);
//    pthread_mutex_init(&mutex, NULL);
//    pthread_cond_init(&condvar, NULL);
//
//    pthread_t producer1, producer2, producer3, consumer1, consumer2;
//
//    Produce_range range1 = {0, 10};
//    pthread_create(&producer1, &attr, producer, static_cast<void *>(&range1));
//    Produce_range range2 = {range1.end, range1.end + 10};
//    pthread_create(&producer2, &attr, producer, static_cast<void *>(&range2));
//    Produce_range range3 = {range2.end, range2.end + 10};
//    pthread_create(&producer3, &attr, producer, static_cast<void *>(&range3));
//
//    int consume_demand1 = 20;
//    int consume_demand2 = 10;
//    pthread_create(&consumer1, &attr, consumer,
//                   static_cast<void *>(&consume_demand1));
//    pthread_create(&consumer2, &attr, consumer,
//                   static_cast<void *>(&consume_demand2));
//
//    pthread_join(producer1, NULL);
//    pthread_join(producer2, NULL);
//    pthread_join(producer3, NULL);
//    pthread_join(consumer1, NULL);
//    pthread_join(consumer2, NULL);
//}

//// 注意某些调用可能会抛出std::system_error
//std::mutex mutex;
//std::condition_variable condvar;
//
//std::queue<int> msgQueue;
//
//void producer(int start, int end)
//{
//    for (int x = start; x < end; x++) {
//        std::this_thread::sleep_for(std::chrono::milliseconds(200));
//        {
//            std::lock_guard<std::mutex> guard(mutex);
//            msgQueue.push(x);
//        }
//        printf("Produce message %d\n", x);
//        condvar.notify_all();
//    }
//}
//
//void consumer(int demand)
//{
//    while (true) {
//        std::unique_lock<std::mutex> ulock(mutex);
//        condvar.wait(ulock, []{ return msgQueue.size() > 0;});
//        // wait的第二个参数使得显式的double check不再必要
//        printf("Consume message %d\n", msgQueue.front());
//        msgQueue.pop();
//        --demand;
//        if (!demand) break;
//    }
//}
//
//
//int main()
//{
//    std::thread producer1(producer, 0, 10);
//    std::thread producer2(producer, 10, 20);
//    std::thread producer3(producer, 20, 30);
//    std::thread consumer1(consumer, 20);
//    std::thread consumer2(consumer, 10);
//
//    producer1.join();
//    producer2.join();
//    producer3.join();
//    consumer1.join();
//    consumer2.join();
//}

////std::move 和 std::forward
//std::move不移动任何东西，后者也不移动，
//仅仅是类型转换函数
//前者无条件参数转换为右值，后者只是在必要情况下
//由于传参可能存在值传递、左值引用、常量左值引用、右值引用等情况，为了保证参数的属性和类型不发生变化，我们需要使用std::forward实现完美转发

////如何自定义内存分配
//当new一个对象时候
//X *px = new X();
//编译器生成：
//void *memory = operator new(sizeof(X));// 得到未经处理的内存
//call string::string()  on *memory;     // 调用初始化函数
//X *px = static_cast<X*>(memory);       // px指针指向生成的对象

//new operator ：new的操作符，理解为关键字，操作原语，生成一个对象或者对象数组
//operator new: 函数，用户自定义
//placement new：operator new的特例，多一个内存地址作为参数
//自定义内存分配两种new操作：
//new operator = operator new + constructor
//重新实现operator new即可。
void * operator new(size_t size) {
//    ...
//    return 内存地址;
}
//节省内存空间，省掉搜索内存表的实际，加快分配，避免内存碎片
//new operator = placement new + constructor
//char *buffer = new char[size]; //buffer以已经分配好的内存
//X *px = new (buffer) X();
//需要手工释放
//px->~x();//调用析构函数
//freeMemory(buffer);//伪代码,释放内存buffer

////进程线程
// - 区别和联系
// 	- 线程是CPU调度的基本单位，同一个进程中可以有多个线程，但至少有一个线程
// 	- 进程是操作系统分配资源的最小单位
// - 一个进程中有哪些数据段
// 	- 代码段（Text Segment）：保存代码
// 	- 静态数据段（Data Segment）：静态数据
// 	- 未初始化数据段（BSS）
// 	- 栈（Stack）：向下增长
// 	- 堆（Heap）：向上增长
// 	- .rodata
// - 同一个进程下的不同线程间有哪些数据段是共享的？
// 	- 除Stack外均共享

////malloc 和 free
// 参考信息：
// 1、malloc与free是C++/C语言的标准库函数，new/delete是C++的运算符。它们都可用于申请动态内存和释放内存。
// 2、对于非内部数据类型的对象而言，光用malloc/free无法满足动态对象的要求。对象在创建的同时要自动执行构造函数，对象在消亡之前要自动执行析构函数。
// 3、由于malloc/free是库函数而不是运算符，不在编译器控制权限之内，不能够把执行构造函数和析构函数的任务强加于malloc/free。因此C++语言需要一个能完成动态内存分配和初始化工作的运算符new，以一个能完成清理与释放内存工作的运算符delete。注意new/delete不是库函数。
// 4、C++程序经常要调用C函数，而C程序只能用malloc/free管理动态内存。
// 5、new可以认为是malloc加构造函数的执行。new出来的指针是直接带类型信息的。而malloc返回的都是void指针。
// 参考信息：
// 必须保证 this 对象是通过 new（不是 new[]、不是 placement new、不是栈上、不是全局、不是其他对象成员）分配的
// 必须保证调用 delete this 的成员函数是最后一个调用 this 的成员函数
// 必须保证成员函数的 delete this 后面没有调用 this 了
// 必须保证 delete this 后没有人使用了

////零copy---减少CPU开销
//mmap调用
//sendfile
//splice

////inline marco
//内联函数通过避免被调用的开销来提高执行效率，尤其是它能够通过调用（“过程化集成”）被编译器优化
//宏是预编译器的输入，然后宏展开之后的结果会送去编译器做语法分析

////多态
//静态多态通过重载、模板来实现；
//动态多态就是通过虚函数来体现的
//每个声明了虚函数或者继承了虚函数的类都会有
//vtbl（virtual table）和vptr（virtual table pointer
//纯虚函数：定义纯虚函数的目的在于，使派生类仅仅只是继承函数的接口
//只有当一个类被用来作为基类的时候，才把析构函数写成虚函数
//虚函数不能内联：内联函数是指在编译期间用被调用的函数体本身来代替函数调用的指令，但是虚函数的“虚”是指“直到运行时才能知道要调用的是哪一个函数
//如果在基类的构造函数中调用了虚函数，并且这个虚函数在某个派生类中被重写了，那么在创建这个派生类的对象时，虚函数调用将只会执行基类版本的虚函数，而不会执行派生类版本的虚函数。这是因为，此时派生类部分的对象还没有被完全构造好，所以不能调用派生类的成员函数
//虚继承：菱形继承问题
//虚函数表是由类维护的，而非单独为每个对象维护的
//同一个类的两个对象使用相同的虚函数表，但它们各自持有自己的指向这个虚函数表的指针，重写 维护新的

////指针
//悬挂指针: 悬挂指针产生于指针所指向的内存已被释放或者失效后，指针本身没有及时更新或清空
//野指针: 野指针通常是指未初始化的指针，它没有被设置为任何有效的地址

////std::priority_queue
//默认情况下使用 std::vector 作为其底层容器，并且使用 std::make_heap、std::push_heap 和 std::pop_heap 算法来维护堆的性质基于堆实现的

////静态链接和动态链接
//静态链接库
//程序编译时，静态库的内容会被复制到最终的可执行文件中
//导致较大的可执行文件大小
//多个程序使用相同的库，每个程序都有自己的副本，这将导致内存的浪费
//动态链接库
//程序在编译时并不复制库中的代码，而是在程序运行时加载库文件
//多个正在运行的程序共享，只需在内存中有一个副本

////对一个对象先malloc后delete这样使用可以吗
//不会调用对象的构造函数和析构函数
//C++中使用 new 和 delete 来动态分配和释放对象的内存，因为它们会正确调用对象的构造函数和析构函数，确保对象的状态正确初始化和正确释放

////内存映射文件
//内存管理功能，它允许应用程序将磁盘上的文件内容映射到进程的地址空间
//好处是可以像访问普通内存那样访问文件数据，通过指针直接读写文件，而不是调用传统的文件I/O操作（如read和write）。这种方式可以提高处理大型文件时的性能，特别是需要频繁随机访问文件部分内容的场景，常见的如mmap

#include <iostream>
#include <list>

class MemoryPool {
public:
    MemoryPool(size_t size, unsigned int count) {
        for (unsigned int i = 0; i < count; ++i) {
            freeBlocks.push_back(new char[size]);
        }
        blockSize = size;
    }

    ~MemoryPool() {
        for (auto block : freeBlocks) {
            delete[] block;
        }
    }

    void* allocate() {
        if (freeBlocks.empty()) {
            throw std::bad_alloc();
        }

        char* block = freeBlocks.front();
        freeBlocks.pop_front();
        return block;
    }

    void deallocate(void* block) {
        freeBlocks.push_back(static_cast<char*>(block));
    }

private:
    std::list<char*> freeBlocks;
    size_t blockSize;
};

// 使用示例
int main() {
    const int blockSize = 32; // 块大小
    const int blockCount = 10; // 块数量
    MemoryPool pool(blockSize, blockCount);

    // 分配内存
    void* ptr1 = pool.allocate();
    void* ptr2 = pool.allocate();

    // 使用ptr1和ptr2...

    // 释放内存
    pool.deallocate(ptr1);
    pool.deallocate(ptr2);

    return 0;
}
