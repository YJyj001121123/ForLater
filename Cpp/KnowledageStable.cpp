
////Crash

//get/set env 线程不安全问题
//占比高
/***
 * backtrace:
 * #00 pc 00000000000d4ec4 /libc.so
 *     getenv +100
 * #01 pc 00000000000dd400 libc.so
 *    tzset_unlocked+40
 * #02 pc 0000000000d827c libc.so
 *    localtime_r+64
 * #03 pc 000000000002d00 
*/
//大部分都是启动崩溃了
//问题散 猜多线程问题 
//getenv是非线程安全的函数
//setenv触发多线程问题---但是找不到setenv的崩溃
//hook
//Hook允许程序员在原始函数执行之前或之后注入自定义代码，以拦截函数的调用并修改其行为
//Hook也可以用于在特定事件发生时触发自定义操作
//函数钩子：在编译时或运行时修改函数的调用指令，从而将控制流转移到hook函数，然后再调用原始函数。
//消息钩子：拦截和处理操作系统发送给应用程序的消息，允许自定义代码对消息进行修改或响应

//拦截了名为setenv_proxy的函数调用 函数执行前后执行一些额外的逻辑。
//记录函数的参数信息以及调用栈信息，这些信息可能用于调试、性能分析或其他目的
int setenv_proxy(const char *name, const char *value, int overwrite) {
    BYTEHOOK_STACK_SCOPE();
    BackTrace * bt = new BackTrace;
    unwind_backtrace(bt);
    if(FdTrackerManager::inst() && FdTrackerManager::inst()->config && FdTrackerManager::inst()->config->javaStackSwitch) {
        BackTraceHelper::inst()->getJavaTraceStr(bt);
    } else {
        bt->java_trace = "";
    }
    FdList::inst()->getNativeTrace(bt, 0, 0);
    LOGI("NativeToolUtil", "setenv_proxy name=%s, value=%s, overwrite=%d", name, value, overwrite);
    LOGI("NativeToolUtil", "%s", bt->native_tombstone);
    LOGI("NativeToolUtil", "%s", bt->java_trace);
    int ret = BYTEHOOK_CALL_PREV(setenv_proxy, name, value, overwrite);
    return ret;
}
//尝试setenv/getenv加读写锁



////Dealloc
//资源获取环可以采用图来存储， 使用有向图来存储。 线程 A 获取线程 B 已占用的锁，则为
//线程 A 指向线程 B。 如何为线程 B 已占用的锁？运行过程线程 B 获取成功的锁。
//检测的原理采用另一个线程定时对图进程检测是否有环的存在

//同一个线程可以多次获取同一个递归锁，不会产生死锁。而如果一个线程多次获取同一个非递归锁，则会产生死锁
//backTrace() 执行线程的回溯操作，用于死锁检测和排查

//auto trace_map = basic::TaskTrace::GetTaskTraceMap(); 获取系统中任务的追踪映射，记录任务队列的执行情况
auto last_num = TaskqueueDeadLockDetect::GetLastExeNum().find(_weak.first);
if (last_num != TaskqueueDeadLockDetect::GetLastExeNum().end()) {
    // 检查是否存在上次执行的任务编号
    if ((now_pick > now_end) && (now_end == last_num->second.first)) {
        // 如果当前任务执行数大于等于上次执行数，并且当前任务完成数等于上次执行数，则认为可能存在死锁
        if (++last_num->second.second > 1) {
            // 发现死锁，根据配置决定是报告死锁还是直接终止程序
            if (deadlock_params_.enable_dead_lock && last_num->second.second > deadlock_params_.dead_lock_time) {
                if (deadlock_params_.enable_dead_lock_notify) {
                    // 报告死锁情况
                } else {
                    // 直接终止程序
                    abortWrapper();
                }
            }
        }
    } else {
        // 死锁解除，重置死锁计数器
        last_num->second.second = 0;
    }
}
//首先检查是否存在上次执行的任务编号，如果存在，则意味着上次执行时的任务信息仍然可用。
//如果当前任务执行数大于等于上次执行数，并且当前任务完成数等于上次执行数，则可能发生了死锁。此时会增加死锁持续时间计数器。
//如果发现死锁并且满足一定持续时间条件，根据配置决定是报告死锁还是直接终止程序
//如果没有发现死锁，重置死锁持续时间计数器，以便在下次检测中重新开始计数


////GTest UT
//https://juejin.cn/post/6844903976765243400

//gtest
// ASSERT_* 系列的断言，当检查点失败时，立即退出单元测试；
// EXPECT_* 系列的断言，当检查点失败时，单元测试还是会继续执行，但结束后会标记所有ECPECT_*失败的用例
// EXPECT_CALL 设置函数调用之后期望的实现，比如直接返回某一个值。该断言后面没有.Times()时，无论函数有没有调用都不会导致失败，如果有.Times()时，不满足.Times()设置的次数时就会导致期望失败

//mock工具的作用是指定函数的行为（模拟函数的行为）。可以对入参进行校验，对出参进行设定，还可以指定函数的返回值。
//Mock是基于多态实现的，gmock是不支持Mock全局函数或者静态成员函数的
//传统的做法是创建一个Wrapper, 用虚方法对这些静态函数进行包裹. 在测试的时候对Wrapper进行Mock便可控制被包裹的静态函数的行为

=
class Calculator {
public:
    virtual ~Calculator() {}
    virtual int Add(int a, int b) const;
};

int Calculator::Add(int a, int b) const {
    return a + b;
}

class MockCalculator : public Calculator {
public:
    MOCK_CONST_METHOD2(Add, int(int a, int b));
};
TEST(CalculatorTest, Add) {
    MockCalculator mockCalculator;

    EXPECT_CALL(mockCalculator, Add(2, 3))
        .WillOnce(Return(5));

    Calculator &calculator = mockCalculator;
    int result = calculator.Add(2, 3);

    EXPECT_EQ(result, 5);
}

class CalculatorTest {
public:
     ~Calculator() {}
    int Add(int a, int b){
        return a+b;
    }
};
TEST(CalculatorTest, Add) {
    Calculator calculator;
    EXPECT_EQ(calculator.Add(2, 3), 5);
    EXPECT_EQ(calculator.Add(-1, 1), 0);

    // You can add more test cases here
}

class A{
public:
　　int Funtion1(B& obj) {
　　　　//do something
　　　　std::string str = “mock non-virtual methods using templates”;
　　　　auto rst = obj.Function2(str);
　　　　//do something
　　}
}

class B{
public:
    int Funtion2(std::string _str){ puts(_str.c_str()); }
}

//在实例化的时候把依赖的类B显式的“注入”进去
template <class T1 >
class  RefactorA{
public:
　　int Funtion1(T1 & obj) {
　　　　//do something
　　　　std::string str = “mock non-virtual methods using templates”;
　　　　auto rst = obj.Function2(str);
　　　　//do something
　　}
}

class  mockB
{
public:
　　MOCK_METHOD1(Funtion2, int (std::string ));
};

class RefactorA_UT : public :: testing::Test
{
protected:
　　virtual void SetUp(){}
　　virtual void TearDown(){}

　　RefactorA < mockB > mockObjA;//实例化模板类
};
 
TEST_F(RefactorA _UT , Funtion1)
{
　　//期望类B的方法Function2被调用至少一次，返回值为100，参数为任意字符串
　　mockB  mockObjB;
　　EXPECT_CALL(mockObjB, Funtion2 (_))
　　.Times(AtLeast(1))
　　.WillOnce(Return(100));

　　auto  rst  =  mockObjA.Function1( mockObjB );//注意这里传入的是mock出来的对象 

　　EXPECT_TRUE( rst );
}

class Foo {
 public:
  ...
  virtual bool Transform(Gadget* g) = 0;

 protected:
  virtual void Resume();

 private:
  virtual int GetTimeOut();
};

class MockFoo : public Foo {
 public:
  ...
  MOCK_METHOD(bool, Transform, (Gadget* g), (override));

  // The following must be in the public section, even though the
  // methods are protected or private in the base class.
  MOCK_METHOD(void, Resume, (), (override));
  MOCK_METHOD(int, GetTimeOut, (), (override));
};


//模拟类将与真实类无关，但包含具有相同签名的方法，而不是与真实类共享公共基类

// A simple packet stream class.  None of its members is virtual.
class ConcretePacketStream {
 public:
  void AppendPacket(Packet* new_packet);
  const Packet* GetPacket(size_t packet_number) const;
  size_t NumberOfPackets() const;
  ...
};

// A mock packet stream class.  It inherits from no other, but defines
// GetPacket() and NumberOfPackets().
class MockPacketStream {
 public:
  MOCK_METHOD(const Packet*, GetPacket, (size_t packet_number), (const));
  MOCK_METHOD(size_t, NumberOfPackets, (), (const));
  ...
};