
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
 * 
 * 动态注册
 * JNI_OnLoad RegisterNatives
 * 
 * 常见方法
 * jint GetVersion()
 * jclass FindClass(const char* name) --根据类的全路径找到相应的 jclass 对象
 * jmethodID GetMethodID(jclass clazz, const char name, const char sig)--获取类中某个非静态方法的ID
*/

////QT开发
//https://blog.csdn.net/qq_33462307/article/details/108998579
//https://zhuanlan.zhihu.com/p/531139706
//https://juejin.cn/post/7338269539539648539
/***
 * Qt的元对象小工具为对象间的交流、运行时类型信息和动态属性小工具提供了警报和槽机制。
 * 元项系统是基于三件事。
 * QObject宏为可以利用元对象工具的项目提供了一个基础宏。
 * 在类声明的个人阶段中的Q_OBJECT宏被用来允许元对象的功能，其中包括动态房屋、指标和槽。
 * 元对象编译器（moc）为每个QObject子类提供了重要的代码，以实现元对象功能。
 * moc工具读取一个C++源文件。如果它发现了一个或更多的包括Q_OBJECTmacro的优雅声明，它就会产生任何其他的C++供应文件，这些文件包含了每个指令的元项代码。
 * 
 * 先调用 moc 和 uic 对 Qt 源文件进行预处理，然后再调用编译器进行编译
*/


////Websocket
//https://chodocs.cn/interview/net/websocket/
//https://blog.csdn.net/cpcpcp123/article/details/121844051
////C++网络编程
//看connect代码
