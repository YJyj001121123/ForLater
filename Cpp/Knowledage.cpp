#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <thread>
#include <list>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <map>


////引用和指针 & *  ｜｜ 值传递、引用传递、指针传递
//指针本身是实体是需要内存空间的，引用只是别名
//指针可以重新赋值，指翔不同的变量；引用初始化后不能更改的
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
//静态全局变量：函数外部声明。只局限在声明它的文件内
static int cnt = 0; //当前文件共享的变量，其他文件无法访问
//静态成员变量：类中，类的所有对象共有的。
//静态成员函数：类的所有对象共享。只能访问静态成员数据、其他静态成员函数和类外部函数。
class StaticClass{
public:
    static int cnt;
    static void increaseCounter(){
        cnt++;
    }
};
int StaticClass::cnt = 0; //需要在类外初始化
void static_member(){
    std::shared_ptr<StaticClass> s1 = std::make_shared<StaticClass>();
    StaticClass s2; //s1 s2共享cnt
    StaticClass::increaseCounter();//不用创建对，直接调用静态成员函数
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
const int *const_a3; //修饰指针所指向的变量，const_a3不能变
int* const const_a4 = nullptr ; // 指针本身为常量，所指向内容可变
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
//静态分配：
//重载new delete 设置private
//动态分配：
//构造析构置为protected 派生类去动态创建

////合成默认构造函数和合成复制构造函数
//合成默认构造函数
//带有默认构造函数的类成员对象：如果一个类没有任何构造函数，但它含有一个成员对象，而后者有默认构造函数，那么编译器就为该类合成出一个默认构造函数。
//带有默认构造函数的基类
//带有虚函数的类、虚基类
//合成的默认构造函数中，只有基类子对象和成员类对象会被初始化。所有其他的非静态数据成员都不会被初始化。
//合成复制构造函数
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
//数组。连续的内存空间，能够高效随机存取，时间复杂度O(1)，插入删除O(n)
//当vector内存不够，重新申请double的内存，复制过去
//迭代器使用后就失效
//list：
//内存空间不是连续的，指针操作。随机存取，时间复杂度O(n)，插入删除O(1)。只能遍历访问。
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
    vector5.push_back(1); //最后插入元素
    vector5.emplace_back(1);
    vector5.pop_back(); //删除最后一个元素
    vector5.erase(vector5.begin()+1, vector5.end()-3); //删除指定范围元素
    vector5.insert(vector5.begin(), 5); //指定位置插入
    vector5.emplace(vector5.begin(),2);
    vector5.insert(vector5.begin()+1,vector2.begin(),vector2.end());//指定位置插入2
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
    auto f = set1.count(2); //查找个数 一般0或1
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
    unset1.count(2); //查找2 有几个  一般0或者1
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

////lambda函数


