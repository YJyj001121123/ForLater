
////EventQueue
//多个生产者一个消费者的场景 ，需要事件队列管理可能存在的事件积压，以及插入取出
std::mutex locker; //对EventQueue的数据成员操作，需要加锁
std::condition_variable             cv_; //条件变量，用来实现不同线程之间的同步，当有新的事件插入队列需要通知其他可能在等待的线程
std::list<Item>                     event_queue_;
bool                                validate_; //初始化信号

//插入事件到链表的尾部并通知可能阻塞等待的线程
void EmplaceBackNotifyOne(Item& item); 
//插入事件到链表头部 。taskqueue如果判断延时任务到执行事件就插入头
void InsertNotifyOne(Item&& item);
//等待一个事件到插入或者收到其他应该退出的信号。如果队列中有事件，，就取出事件通过引用参数container返回调用方
//阻塞当前调用线程，并从EventQueue获取未被执行的任务
size_t Wait(Item& container, std::function<bool()> interrupt = nullptr); 
// 在 EventQueue 关闭的情况下，仍然会取出所有遗留未处理的事件。
// Wait 退出的条件有三个：
//   - validate_ 被设置为 false；
//   - 在 Wait 被唤醒的条件下，如果执行 interrupt 方法返回 true，Wait 结束返回；
//   - event_queue_ 的 size 大于 0；
//唤醒一次 Wait 阻塞，会改变队列的状态，然后调用 cv_.notify_all() 通知其他正阻塞在 Wait 的线程
void BreakOne(std::function<void()> breaker = nullptr); //
//在 TaskQueue 的线程主函数中，如果没有取到任务，会执行 EventQueue::WaitFor 操作，
//WaitFor 的 interrupt 就是判断延时任务的队列长度是否发生变化。
//TaskQueue::AsyncDelayTask() 方法中会调用 EventQueue::BreakOne 方法，插入一个延时任务


//实际使用我们不是单纯使用信号量进行状态同步的，而是将对象放入缓冲区，并通知消费者线程检查缓冲区

template < typename LockType, LockType ValidateValue >
class PendingLockT {
public: 
    bool Wait() {
        LockType _r;
        m_epl.Wait(_r);
        return _r == ValidateValue;
    }
    bool WaitUntil( typename EventQueue<LockType>::IntervalType d ) {
        LockType _r;
        auto _f = m_epl.WaitFor(d, _r);
        return (_f == EventQueue<LockType>::kGetEvent && _r == ValidateValue);
    }
    void JobDone() { m_epl.NotifyOne(ValidateValue); }

    PendingLockT() { m_epl.Enable(); }
    PendingLockT(const PendingLockT &) = delete;
    PendingLockT(PendingLockT&&) = delete;
    PendingLockT& operator= (const PendingLockT&) = delete;
    PendingLockT& operator= (PendingLockT&&) = delete;
protected:
    EventQueue<LockType> m_epl;
};

typedef PendingLockT<bool, true> PendingLock;

//线程1需要在线程2中执行一个同步任务，实现机制就是通过定义一个std::shared_ptr<PendingLockT>对象，
//并用这个对象构造一个 PendingLockKeeper对象，传递给线程2，
//任务执行完时，PendingLockKeeper 析构并调用 PendingLockT::JobDone() 方法，通知正在等待的线程1.





////TaskQueue
//是EventQueue的封装 
//业务调用sdk 的接口所在的线程对我们而言是不确定的，需要将业务方对接口的调用转到内部的确定线程，保证数据同步

//所有任务统一封装为std::function<void()> 放到evenqueue管理



////KVStorage
map 
////CacheUse
//windows 写入注册表
//android  SharedPreferences 
//apple  NSUserDefaults

//crc 加解密 meta 添加字段 conut
//getvalue 取cnt字段 0-2 不删除磁盘的数据 cnt++  超过3就返回了异常情况/读取超过三次

//进房的配置写回  置0
//销毁引擎时候认为配置可用，写回

// - 线上版本稳定，突然异常就是配置变更的问题。
// - SDK一直在本地会进行持久化配置存储，变更才会更新
// - 时间差风险点：当用户缓存了一份错误的配置，且在下一次获取正确配置之前就使用了错误配置，就会导致用户持续crash，且没有恢复手段


////MmapUse
//写入 mmap 区域：mmap 是一种将文件或设备映射到进程地址空间的机制
//由于写入 mmap 区域实际上是对文件进行写操作，因此在大多数情况下，写入 mmap 区域的性能要比传统的读取文件、修改数据、写回文件的方式要高，特别是对于大文件和频繁写入的情况
//对 mmap 区域的写入操作可以通过操作系统的缓存机制进行异步写入，因此写入操作可能不会立即反映在文件中。如果需要立即同步到文件中，可以使用 msync() 函数进行强制同步
//写入 mmap 区域实际上是对文件进行写操作，只是你直接在内存中操作，而不是通过文件句柄
//写入普通内存是直接在进程的地址空间中操作数据。这种操作没有直接影响到文件内容，除非你将内存中的数据写入到文件中

////WeakUse
//保证异步执行的安全性。
//要实现这种机制，需要能够检测对象的生命周期，但又不影响对象的生命周期，比较直观的想法是在对象析构的时候发出通知

////Kcp
//https://zhuanlan.zhihu.com/p/581526921