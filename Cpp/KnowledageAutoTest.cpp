
////CMake 
//https://juejin.cn/post/7336092523771969575
//https://blog.51cto.com/u_15294398/2990236
//https://learnku.com/articles/57334
/***
 * CMake
 * 跨平台的自动化构建系统，主要用来管理软件构建的过程，它使用一个名为CMakeLists.txt的配置文件来指导编译和链接的过程。CMake支持多种编译器和开发环境，可以生成标准的构建文件，如Makefile或者Visual Studio的项目文件。
 * message(STATUS "")
 * file(GLOB_RECURSE XX "")
 * target_link_libraries
 * target_include_directories
 * add_executable
 * add_custom_command(TARGET ${xx} POST_BUILD COMMAND ${CMAKE_COMMAND} -E cpoy
 * add_custom_target
 * 
 * find_package
 * 使用CMake的set命令来设置编译器标志，以及add_compile_options来添加编译选项
 * 构建类型（如Debug和Release），我会使用CMAKE_BUILD_TYPE变量
 * 
 * 编译器标志
 * 传递给编译器的命令行参数，用于控制编译器的行为。
 * 这些标志通常影响编译器的行为、优化级别、警告级别、目标平台等。
 * 对于GCC编译器，使用-O标志来指定优化级别；
 * 而对于Clang编译器，则使用-O标志，但还可以使用-Weverything标志来启用所有警告
 * 
 * 编译选项
 * 用于控制编译过程的设置，包括源代码的处理、目标文件的生成、链接器的行为等。
 * 编译选项通常由构建系统（如Makefile、CMake等）提供
 * -I<include_path>（指定头文件搜索路径）、-D<macro>（定义宏）、-std=c++11（指定C++语言标准）、-pthread（启用POSIX线程支持）等
 * 
 * 模块化设计：将CMake代码分解为多个模块，每个模块负责一个特定的功能或组件。
 * 使用函数和宏：将重复的代码封装到函数或宏中，减少代码冗余。
 * 代码注释：在CMake代码中添加充足的注释，解释复杂的逻辑和重要的决策。
 * 遵循编码规范：制定并遵循一套CMake代码的编码规范，确保代码风格的一致性。
 * 定期代码审查：定期进行代码审查，确保代码质量并分享最佳实践。
 * 文档化：创建详细的文档，描述CMake代码的结构、功能和使用方法。
 * 
 * 并行构建：利用make -j或其他构建工具的并行构建功能，充分利用多核CPU。
 * 分离代码：将项目分解为多个库和可执行文件，只有发生变化的部分需要被重新构建。
 * 预编译头文件：对于C++项目，使用预编译头文件来减少编译时间。
 * ccache或其他缓存工具：使用ccache或其他编译缓存工具来缓存编译结果，避免重复编译。
 * 优化编译选项：合理使用编译优化选项，避免使用过度的优化设置。
 * 
 * Linux 编译C
 * gcc xx.c -o hello
*/

////JNI
//https://blog.csdn.net/afei__/article/details/84889135
//https://blog.csdn.net/yyg_2015/article/details/72229892
/***
 * JNI 
 * Java Native Interface 的缩写，即 Java 的本地接口。
 * 与Android无关,属于JAVA
 * 
 * NDK
 * Native Development Kit 的缩写，是 Android 的工具开发包
 * 更方便和快速开发 C/C++ 的动态库，并自动将动态库与应用一起打包到 apk
 * 
 * JNI 是实现的目的，NDK 是 Android 中实现 JNI 的手段。
 * 
 * JavaVM
 * Java程序的运行环境，负责加载、解释和执行Java字节码
 * 提供了内存管理、垃圾回收、线程管理、安全性等功能，以确保Java程序的正确运行。
 * 一个进程只有一个 JVM
 * 
 * JNI允许Java程序通过调用本地方法（Native Method）来执行本地代码
 * 而本地方法的执行是在JVM之外的，通常是由本地编译器（如GCC、Visual C++等）生成的本地库文件提供支持
 * JNI提供了一种接口，允许Java程序通过特定的函数调用来访问本地代码。
 * 这些本地方法在被调用时，JVM会将控制权转移到本地代码，然后在本地代码执行完毕后，将控制权再次返回给JVM。
 * 
 * JNIEnv
 * 表示了一个JVM实例中的运行环境，它提供了一组函数指针，用于执行Java代码和处理Java对象
 * Java 调用 native 语言的环境，封装了几乎全部 JNI 方法的指针
 * JNIEnv 只在创建它的线程生效，不能跨线程传递，不同线程的 JNIEnv 彼此独立。
 * 通过 AttachCurrentThread 和 DetachCurrentThread 方法将 native 的线程与 JavaVM 关联和解除关联。
 * 
 * 全局引用
 * NewGlobalRef
 * DeleteGlobalRef
 * 全局引用能在多个线程中被使用，且不会被 GC 回收，只能手动释放
 * JNI 线程间数据互相访问
 * 
 * 弱全局引用
 * NewWeakGlobalRef
 * DeleteWeakGlobalRef
 * 类似于全局引用，唯一的区别是它不会阻止被 GC 回收
 * 
 * 局部引用
 * NewLocalRef
 * DeleteLocalRef
 * 局部引用只在创建它的 native 方法中有效，包括其调用的其它函数中有效
 * 会在 native 方法返回时全部自动释放
 * 
 * 静态注册
 * JNIEXPORT
 * JNICALL
 * Java + 包名 + 类名 + 方法名 形式的函数
 * 静态注册中，JNI 方法是直接在 C/C++ 代码中声明和定义的
 * 
 * 动态注册
 * JNI_OnLoad RegisterNatives
 * JNI 方法的注册发生在 Java 代码中，通过 JNI 函数在运行时注册 JNI 方法
 * 
 * 常见方法
 * jint GetVersion()
 * jclass FindClass(const char* name) --根据类的全路径找到相应的 jclass 对象
 * jmethodID GetMethodID(jclass clazz, const char name, const char sig)--获取类中某个非静态方法的ID
 * 
 * 怎么写的JNI
 * public static native void nativeLoadSDK(String sdkName);
 * @CalledByNative
 * 
 * JNI_GENERATOR_EXPORT 导出符号 
 * 定义了一个JNI本地方法
 * 
*/

////QT开发
//https://blog.csdn.net/qq_33462307/article/details/108998579
//https://zhuanlan.zhihu.com/p/531139706
//https://juejin.cn/post/7338269539539648539
/***
 * Qt跨平台的C++开发框架
 * moc全称是Meta-Object Compiler，也就是“元对象编译器”。如果发现Q_OBJECT需要生成另外源文件实现
 * gcc前 moc把扩展语法处理掉
 * src--->moc uic 
 * 
 * 消息系统  QEvent
 * 信号---对象订阅事件 槽--另外对象关注 回调函数 connec 注册观察者
 * 
 * 初始化：
 * 控件初始化放在构造函数
 * 子控件随父控件销毁
 * 
 * ui文件---xml
 * 如果需要设置属性 自定义代码
 * Qt::WA_NativeWindow 是告诉 Qt 在部件上创建一个原生窗口的属性
 * QThread
 * 
 * 
 * Qt的元对象小工具为对象间的交流、运行时类型信息和动态属性小工具提供了警报和槽机制。
 * 元项系统是基于三件事。
 * QObject宏为可以利用元对象工具的项目提供了一个基础宏。
 * 在类声明的个人阶段中的Q_OBJECT宏被用来允许元对象的功能，其中包括动态房屋、指标和槽。
 * 元对象编译器（moc）为每个QObject子类提供了重要的代码，以实现元对象功能。
 * moc工具读取一个C++源文件。如果它发现了一个或更多的包括Q_OBJECTmacro的优雅声明，它就会产生任何其他的C++供应文件，这些文件包含了每个指令的元项代码。
 * 
 * 先调用 moc 和 uic 对 Qt 源文件进行预处理，然后再调用编译器进行编译
 * 
 * 信号槽机制
 * MOC查找头文件中的signal与slots，标记出信号槽。
 * 将信号槽信息储存到类静态变量staticMetaObject中，并按照声明的顺序进行存放，建立索引。
 * connect链接，将信号槽的索引信息放到一个双向链表中，彼此配对。
 * emit被调用，调用信号函数，且传递发送信号的对象指针，元对象指针，信号索引，参数列表到active函数。
 * active函数在双向链表中找到所有与信号对应的槽索引，根据槽索引找到槽函数，执行槽函数
 * 
 * 如何打包
 * windeployqt
 * 
 * 信号槽的链接方式
 * Qt::AutoConnection  默认值，使用这个值则连接类型会在信号发送时决定
 * Qt::DirectConnection 接收者和发送者在同一个线程，则自动使用Qt::DirectConnection类型
 * Qt::QueuedConnection 接收者和发送者不在一个线程，则自动使用Qt::QueuedConnection类型
 * Qt::BlockingQueuedConnection 多线程间需要同步的场合
 * Qt::UniqueConnection 避免了重复连接。
 * 
 * Qt的D指针与Q指针
 * D
 * 一种约定，用于实现对象的内部数据隐藏和封装
 * 通常是一个私有成员变量，用于指向对象的私有数据。通过将私有数据放在类的私有部分
 * 可以隐藏对象的内部实现细节，从而提高代码的安全性和可维护性
 * 指向一个包含所有数据的私有数据结构体
 * 私有的结构体可以随意改变，而不需要重新编译整个工程项目
 * 隐藏实现细节
 * 头文件中没有任何实现细节，可以作为API使用原本在头文件的实现部分转移到乐源文件，
 * 所以编译速度有所提高
 * Q
 * 一种智能指针，用于管理动态分配的对象内存
 * 私有的结构体中储存一个指向公有类的Q指针。
 * Qt中的一个类常用一个PrivateXXX类来处理内部逻辑，使得内部逻辑与外部接口分开，
 * 这个PrivateXXX对象通过D指针来访问；在PrivateXXX中有需要引用Owner的内容，通过Q指针来访问。
 * 由于D和Q指针是从基类继承下来的，子类中由于继承导致类型发生变化，
 * 需要通过static_cast类型转化，所以DPTR()与QPTR()宏定义实现了转换
 * 
 * QtCore：提供了 Qt 的核心功能，例如基本的非 GUI 类、线程和事件处理等。
 * QtGui：提供用户界面（UI）类，例如窗口部件、按钮、标签等。此外，它还包含 QPainter 和 QPalette 等绘图和调色板类
 * QtWidgets：是 QtGui 模块的子集，提供了一套完整的可视化 UI 控件库，例如按钮、文本编辑器、表格等，用于构建跨平台的桌面应用程序
 * QtNetwork：提供网络编程类，用于创建 TCP 和 UDP 客户端和服务器，以及处理套接字和 HTTP 请求。
 * QtSql：提供简单易用的数据库访问 API，用于在 Qt 中连接、查询和操作数据源中的数据
 * 
 * 
 * 
*/
#include <QObject>
class Counter : public QObject
{
    Q_OBJECT

public:
    Counter() { m_value = 0; }

    int value() const { return m_value; }

public slots: //回调函数
    void setValue(int value);

signals:
    void valueChanged(int newValue);

private:
    int m_value;
};
void Counter::setValue(int value)
{
    if (value != m_value) {
        m_value = value;
        emit valueChanged(value);  //emit 触发事件
    }
}
int main(){
    Counter a,b;
    QObject::connect(&a, &Counter::valueChanged, &b, &Counter::setValue); 
    //a的触发这个valuechanged 就会触发b的setvalue
    a.setValue(12); // a == 12  b==12

    QApplication app();
    Qwidget windows;

}

////Websocket
//https://chodocs.cn/interview/net/websocket/
//https://blog.csdn.net/cpcpcp123/article/details/121844051
/***
 * 全双工 应用层协议 允许服务端向客户端发送 基于 TCP
 * 一次握手就可以连接
 * HTTP兼容
 * 可以发送文本也可以二进制
 * 协议标识符：ws
 * WebSocket 在建立握手时，数据是通过 HTTP 传输的。但是建立之后，在真正传输时候是不需要 HTTP 协议的
 * 
 * websocket 超时没有消息自动断开连接
 * 需要知道服务端设置的超时时长是多少，在小于超时时间内发送心跳包
 * 客户端每隔一个时间间隔发生一个探测包给服务器
 * 客户端发包时启动一个超时定时器,等待服务端的回应，判断服务器是否正常
 * 
*/

////C++网络编程
//看connect代码
//设置注册信息
//设置PingLost 超时时间
//设置连接状态监听器
//心跳
//重连
//回调
//事件处理

//open url 和回调

//emit 发送事件。传入事件名称和数据，并将其发送到 WebSocket 服务器
//on 注册时间处理函数
#include <iostream>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/client.hpp>
#include <mutex>

class WebSocketClient {
public:
    WebSocketClient() {
        client_.init_asio();
        client_.set_open_handler(std::bind(&WebSocketClient::onOpen, this, std::placeholders::_1));
        client_.set_message_handler(std::bind(&WebSocketClient::onMessage, this, std::placeholders::_1, std::placeholders::_2));
        client_.set_close_handler(std::bind(&WebSocketClient::onClose, this, std::placeholders::_1));
    }

    void connect(const std::string& uri) {
        websocketpp::lib::error_code ec;
        client::connection_ptr con = client_.get_connection(uri, ec);

        if (ec) {
            std::cout << "Error: " << ec.message() << std::endl;
            return;
        }

        client_.connect(con);
    }

    void send(const std::string& message) {
        websocketpp::lib::error_code ec;
        client_.send(hdl_, message, websocketpp::frame::opcode::text, ec);
        if (ec) {
            std::cout << "Error: " << ec.message() << std::endl;
        }
    }

    void close() {
        client_.close(hdl_, websocketpp::close::status::normal, "Closing connection");
    }

private:
    typedef websocketpp::client<websocketpp::config::asio_client> client;
    typedef websocketpp::connection_hdl connection_hdl;
    typedef client::message_ptr message_ptr;

    client client_;
    connection_hdl hdl_;
    std::mutex mutex_;

    void onOpen(connection_hdl hdl) {
        std::lock_guard<std::mutex> lock(mutex_);
        hdl_ = hdl;
        std::cout << "Connected!" << std::endl;
    }

    void onMessage(connection_hdl hdl, message_ptr msg) {
        std::lock_guard<std::mutex> lock(mutex_);
        std::cout << "Received: " << msg->get_payload() << std::endl;
    }

    void onClose(connection_hdl hdl) {
        std::lock_guard<std::mutex> lock(mutex_);
        std::cout << "Disconnected!" << std::endl;
    }
};

int main() {
    WebSocketClient client;
    client.connect("ws://localhost:9000");

    std::string message;
    while (true) {
        std::cout << "Enter message (or 'quit' to exit): ";
        std::getline(std::cin, message);

        if (message == "quit") {
            break;
        }

        client.send(message);
    }

    client.close();
    return 0;
}
