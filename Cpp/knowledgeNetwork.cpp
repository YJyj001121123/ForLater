
////网络异步IO https://www.expoli.tech/articles/2023/03/25/what-is-linux-epoll
//同步异步是针对应用程序和内核的交互而言的。
//同步是指用户进程触发IO操作并等待或轮询检查IO操作是否就绪，接着阻塞的完成IO操作
//异步是指用户进程触发IO操作之后就做其它的事情，当IO操作完成之后会得到通知(如读取网络数据，此时数据已经由内核拷贝到用户缓冲区，用户进程可以直接使用)。
//Reactor模式用于同步IO，Proactor模式用于异步IO

////多路复用
//linux系统中，实际上所有的I/O设备都被抽象为了文件这个概念，一切皆文件，Everything is File，磁盘、网络数据、终端，甚至进程间通信工具管道pipe等都被当做文件对待
//多路: 指的是多个socket网络连接;
//复用: 指的是复用一个线程、使用一个线程来检查多个文件描述符（Socket）的就绪状态
//阻塞IO:进程阻塞挂起不消耗CPU资源，能及时响应每个操作；实现难度低，适用并发量小的网络应用开发，不适用并发量大的应用，因为一个请求IO会阻塞进程，所以每请求分配一个处理进程（线程）去响应，系统开销大
//非阻塞式I/O:进程轮询（重复）调用，消耗CPU的资源
//信号驱动IO:回调机制，向内核注册一个信号处理函数
//异步IO:Proactor模式

//select
//当用户process调用select的时候，select会将需要监控的readfds集合拷贝到内核空间
//（假设监控的仅仅是socket可读），
//然后遍历自己监控的skb(SocketBuffer)，
//挨个调用skb的poll逻辑以便检查该socket是否有可读事件，
//遍历完所有的skb后，如果没有任何一个socket可读，
//那么select会调用schedule_timeout进入schedule循环，
//使得process进入睡眠。如果在timeout时间内某个socket上有数据可读了，
//或者等待timeout了，则调用select的process会被唤醒，
//接下来select就是遍历监控的集合，挨个收集可读事件并返回给用户了
int select(
    int nfds,
    fd_set *readfds,
    fd_set *writefds,
    fd_set *exceptfds,
    struct timeval *timeout);
// nfds:监控的文件描述符集里最大文件描述符加1
// readfds：监控有读数据到达文件描述符集合，传入传出参数
// writefds：监控写数据到达文件描述符集合，传入传出参数
// exceptfds：监控异常发生达文件描述符集合, 传入传出参数
// timeout：定时阻塞监控时间，3种情况
//  1.NULL，永远等下去
//  2.设置timeval，等待固定时间
//  3.设置timeval里时间均为0，检查描述字后立即返回，轮询

/* 
* select服务端伪码
* 首先一个线程不断接受客户端连接，并把socket文件描述符放到一个list里。
*/
while(1) {
  connfd = accept(listenfd);
  fcntl(connfd, F_SETFL, O_NONBLOCK);
  fdlist.add(connfd);
}
/*
* select函数还是返回刚刚提交的list，应用程序依然list所有的fd，只不过操作系统会将准备就绪的文件描述符做上标识，
* 用户层将不会再有无意义的系统调用开销。
*/
struct timeval timeout;
int max = 0;  // 用于记录最大的fd，在轮询中时刻更新即可
// 初始化比特位
FD_ZERO(&read_fd);
while (1) {
    // 阻塞获取 每次需要把fd从用户态拷贝到内核态
    nfds = select(max + 1, &read_fd, &write_fd, NULL, &timeout);
    // 每次需要遍历所有fd，判断有无读写事件发生
    for (int i = 0; i <= max && nfds; ++i) {
        // 只读已就绪的文件描述符，不用过多遍历
        if (i == listenfd) {
            // 这里处理accept事件
            FD_SET(i, &read_fd);//将客户端socket加入到集合中
        }
        if (FD_ISSET(i, &read_fd)) {
            // 这里处理read事件
        }
    }
}
//每次调用select，都需要把被监控的fds集合从用户态空间拷贝到内核态空间，高并发场景下这样的拷贝会使得消耗的资源是很大的
//能监听端口的数量有限
//被监控的fds集合中，只要有一个有数据可读，整个socket集合就会被遍历一次调用sk的poll函数收集可读事件

//poll
//poll的实现和select非常相似，只是描述fd集合的方式不同
struct pollfd {
　　 int fd;           /*文件描述符*/
　　 short events;     /*监控的事件*/
　　 short revents;    /*监控事件中满足条件返回的事件*/
};
int poll(struct pollfd *fds, unsigned long nfds, int timeout);  
//不使用fd_set
poll服务端实现伪码：
struct pollfd fds[POLL_LEN];
unsigned int nfds=0;
fds[0].fd=server_sockfd;
fds[0].events=POLLIN|POLLPRI;
nfds++;
while {
    res=poll(fds,nfds,-1);
    if(fds[0].revents&(POLLIN|POLLPRI)) {
        //执行accept并加入fds中，nfds++
        if(--res<=0) continue
    }
    //循环之后的fds
    if(fds[i].revents&(POLLIN|POLLERR )) {
        //读操作或处理异常等
        if(--res<=0) continue
    }
}
//epoll
//epoll最大的好处在于它不会随着监听fd数目的增长而降低效率
//当创建好epoll句柄后，它就是会占用一个fd值，在linux下如果查看/proc/进程id/fd/，是能够看到这个fd的，所以在使用完epoll后，必须调用close()关闭，否则可能导致fd被耗尽
int epoll_create(int size); //size 是监听的数目大小
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event); 
//epoll 的事件注册函数，它不同于 select() 是在监听事件时告诉内核要监听什么类型的事件，而是在这里先注册要监听的事件类型。
//epfd epoll 专用的文件描述符，epoll_create()的返回值
//op: EPOLL_CTL_ADD：注册新的 fd 到 epfd 中； EPOLL_CTL_MOD：修改已经注册的fd的监听事件； EPOLL_CTL_DEL：从 epfd 中删除一个 fd；
//fd:需要监听的文件描述符
//event:event
// events可以是以下几个宏的集合：
// EPOLLIN ：表示对应的文件描述符可以读（包括对端 SOCKET 正常关闭）；
// EPOLLOUT：表示对应的文件描述符可以写；
// EPOLLPRI：表示对应的文件描述符有紧急的数据可读（这里应该表示有带外数据到来）；
// EPOLLERR：表示对应的文件描述符发生错误；
// EPOLLHUP：表示对应的文件描述符被挂断；
// EPOLLET ：将 EPOLL 设为边缘触发(Edge Trigger)模式，这是相对于水平触发(Level Trigger)来说的。
// EPOLLONESHOT：只监听一次事件，当监听完这次事件之后，如果还需要继续监听这个 socket 的话，需要再次把这个 socket 加入到 EPOLL 队列里
int epoll_wait(int epfd, struct epoll_event * events, int maxevents, int timeout); 
//等待事件的产生，收集在 epoll 监控的事件中已经发送的事件，类似于 select() 调用
//events: 分配好的 epoll_event 结构体数组，epoll 将会把发生的事件赋值到events 数组中（events 不可以是空指针，内核只负责把数据复制到这个 events 数组中，不会去帮助我们在用户态中分配内存）
//maxevents: maxevents 告之内核这个 events 有多少个 
//timeout: 超时时间，单位为毫秒，为 -1 时，函数为阻塞

//epoll 为什么比select、poll更高效？
// epoll 采用红黑树管理文件描述符
// 从上图可以看出，epoll使用红黑树管理文件描述符，红黑树插入和删除的都是时间复杂度 O(logN)，不会随着文件描述符数量增加而改变。
// select、poll采用数组或者链表的形式管理文件描述符，那么在遍历文件描述符时，时间复杂度会随着文件描述的增加而增加。
// epoll 将文件描述符添加和检测分离，减少了文件描述符拷贝的消耗
// select&poll 调用时会将全部监听的 fd 从用户态空间拷贝至内核态空间并线性扫描一遍找出就绪的 fd 再返回到用户态。下次需要监听时，又需要把之前已经传递过的文件描述符再读传递进去，增加了拷贝文件的无效消耗，当文件描述很多时，性能瓶颈更加明显。
// 而epoll只需要使用epoll_ctl添加一次，后续的检查使用epoll_wait，减少了文件拷贝的消耗。

//select，poll，epoll都是IO多路复用机制，即可以监视多个描述符，一旦某个描述符就绪（读或写就绪），能够通知程序进行相应读写操作。 但select，poll，epoll本质上都是同步I/O，因为他们都需要在读写事件就绪后自己负责进行读写，也就是说这个读写过程是阻塞的，而异步I/O则无需自己负责进行读写，异步I/O的实现会负责把数据从内核拷贝到用户空间。
// select，poll实现需要自己不断轮询所有fd集合，直到设备就绪，期间可能要睡眠和唤醒多次交替。而epoll其实也需要调用epoll_wait不断轮询就绪链表，期间也可能多次睡眠和唤醒交替，但是它是设备就绪时，调用回调函数，把就绪fd放入就绪链表中，并唤醒在epoll_wait中进入睡眠的进程。虽然都要睡眠和交替，但是select和poll在“醒着”的时候要遍历整个fd集合，而epoll在“醒着”的时候只要判断一下就绪链表是否为空就行了，这节省了大量的CPU时间。这就是回调机制带来的性能提升。
// select，poll每次调用都要把fd集合从用户态往内核态拷贝一次，并且要把current往设备等待队列中挂一次，而epoll只要一次拷贝，而且把current往等待队列上挂也只挂一次（在epoll_wait的开始，注意这里的等待队列并不是设备等待队列，只是一个epoll内部定义的等待队列）。这也能节省不少的开销。



////如何打断异步IO


////KCP协议 https://juejin.cn/post/7204770304400490555
//kcp快速可靠的协议，基于UDP
//能以比 TCP 浪费 10%-20% 带宽的代价，换取平均延迟降低 30%-40%，最大延迟降低 3 倍的传输速度。
//纯算法实现，并不负责底层协议（如 UDP）的收发，需要使用者自己定义下层数据包的发送方式，并以 callback 的方式提供给 KCP。
//包括时钟也需要外部传递进来，内部不会有任何一次系统调用
//KCP 整个协议的实现只ikcp.h和ikcp.c两个源文件，可以方便的集成到用户自己的协议栈中。
//比如你实现了一个 P2P，或者某个基于 UDP 的协议
//而缺乏一套完善的 ARQ（自动重传请求，Automatic Repeat-reQuest）实现
// 那么简单的拷贝这两个文件到现有项目中，稍加改造适配，即可使用。
//
//TCP为流量设计（每秒可以传输的数据量），追求利用带宽。TCP信道是流速很慢但是每秒流量大的运河
//KCP为流速设计（单个数据包从一端发送到另一端需要的时间）。KCP小激流
//KCP两种模式正常模式和快速模式（多种策略）ikcp_nodelay
//
///KCP特性：
//RTO（重传超时时间）不会翻倍
//TCP 超时计算是 RTOx2，这样连续丢三次包就变成 RTOx8
//KCP 启动快速模式后不 x2，只是 x1.5（实验证明 1.5 这个值相对比较好），提高了传输速度。
//选择性重传：
//TCP 丢包时会对丢的那个包开始以后的数据全部重传，KCP 则是选择性重传，只重传真正丢失的数据包
//快速重传：
//发送端发送了1,2,3,4,5几个包，然后收到远端的 ACK：1,3,4,5，当收到 ACK3时，KCP 知道2被跳过 1 次，
//收到 ACK4时，知道2被跳过了 2 次
//当次数大于等于设置的 resend 的值的时候，此时可以认为 2号丢失，不用等待超时，直接重传2号包，大大改善了丢包时的传输速度。
//这就是 KCP 的快速重传机制
//ACK + UNA
//UNA（表示此编号之前的所有包已收到，如 TCP）和 ACK（表示此编号包已收到）
//只用 UNA 会导致丢包后只能全部重传，只用 ACK 则丢包后维护成本太高（某个中间包丢掉后，可能需要维护比较多的后续包的缓存），以往协议都是二选其一，
//KCP 协议中，除了单独的 ACK 包外，其他所有包都有 UNA 信息
//
///基本使用
//KCP 的 input/output 方法用来对接下层的 UDP 收发模块
//ikcp_send ickp_recv提供给上层逻辑调用实现

//KCP 的发送和接收单元是 segment，即应用层的数据可能会拆分成多个 segment 发送。
//conv：会话编号。
//cmd：segment 类型，IKCP_CMD_ACK、IKCP_CMD_PUSH、IKCP_CMD_WASK、IKCP_CMD_WINS。
//frg：是否最后一个 segment。0：数据包的最后一个 segment；1：数据包的中间切片 segment。
//sn：下一个待发 segment 的序号。
//una：待接收消息序号，表示这之前的所有 segment 都收到了。
//len：segment 数据长度，不包含头。

///发送端
//在发送端应用层通过 kcp_send 发送数据，KCP 会把用户数据拆分 KCP 数据包，通过 kcp_output 再以 UDP 包的方式发送出去。

//创建kcp对象
// 初始化 kcp对象，conv为一个表示会话编号的整数，和tcp的 conv一样，通信双
// 方需保证 conv相同，相互的数据包才能够被认可，user是一个给回调函数的指针
ikcpcb *kcp = ikcp_create(conv, user);

//设置回调函数
// KCP的下层协议输出函数，KCP需要发送数据时会调用它
// buf/len 表示缓存和长度
// user指针为 kcp对象创建时传入的值，用于区别多个 KCP对象
int udp_output(const char *buf, int len, ikcpcb *kcp, void *user)
{
    ....
}
// 设置回调函数
ikcp_setoutput(kcp, udp_output);

//发送数据
int ikcp_send(ikcpcb *kcp,const char*buffer, int length)

//循环调用update
// 以一定频率调用 ikcp_update来更新 kcp状态，并且传入当前时钟（毫秒单位）
// 如 10ms调用一次，或用 ikcp_check确定下次调用 update的时间不必每次调用
ikcp_update(kcp, millisec);

//接收端
//创建kcp对象

//读取一个下层的数据包、
//收到一个下层数据包（比如UDP包）时需要调用：
ikcp_input(kcp, received_udp_packet, received_udp_size);

//KCP数据还原成发送端发送的 buffer 数据给应用层
int ikcp_recv(ikcpcb *kcp,const char*buffer, int length)

///内存分配器
//默认 KCP 协议使用 malloc/free 进行内存分配释放，如果应用层接管了内存分配，可以用 ikcp_allocator 来设置新的内存分配器
ikcp_allocator(my_new_macllo,my_new_delete)

///前向纠错
//进一步提高传输速度，下层协议也许会使用前向纠错技术。
//前向纠错会根据冗余信息解出原始数据包。
//这里就需要注意，相同的原始数据包不要两次 input 到 KCP，
//否则将会导致 KCP 以为对方重发了，这样会产生更多的 ACK 占用额外带宽。

//管理大规模链接
//需要同时管理大规模的 KCP 连接（比如大于 3000 个），
//比如你正在实现一套类 epoll 的机制，
//那么为了避免每秒钟对每个连接大量调用 ikcp_update，
//我们可以使用 ikcp_check来大大减少 ikcp_update 调用的次数。
//ikcp_check 返回值会告诉你需要在什么时间点再次调用 ikcp_update（如果中途没有 ikcp_send、ikcp_input 的话，否则中途调用了 ikcp_send、ikcp_input 的话，需要在下一次 interval 时调用 ikcp_update）。
//标准顺序是每次调用了 ikcp_update 后，
//使用 ikcp_check 决定下次什么时间点再次调用 ikcp_update，
//而如果中途发生了 ikcp_send、ikcp_input 的话，
//在下一轮 interval 立马调用 ikcp_update 和 ikcp_check。
//使用该方法，原来在处理 2000 个 KCP 连接且每个连接每 10ms 调用一次 update，改为 check 机制后，CPU 从 60% 降低到 15%。

///避免缓存积累延迟
//当你持续调用 ikcp_send，首先会填满 KCP 的 snd_buf，
//如果 snd_buf 的大小超过发送窗口 snd_wnd 限制，则会停止向 snd_buf 里追加数据包，
//只会放在 snd_queue里面滞留着，等待 snd_buf 有新位置了
//（因为收到远端 ACK/UNA 而将历史包从 snd_buf中移除）
//才会从 snd_queue 转移到 snd_buf，等待发送。
//TCP 发送窗口满了不能发送了，
//会阻塞住或者 EAGAIN/EWOULDBLOCK；KCP 发送窗口满了，
//ikcp_send 并不会给你返回 -1，而是让数据滞留在 snd_queue 里等待有能力时再发送。
//为什么 KCP 在发送窗口满的时候不返回错误呢？这个问题当年设计时权衡过，如果返回希望发送时返回错误的 EAGAIN/EWOULDBLOCK 你势必外层还需要建立一个缓存，等到下次再测试是否可以 send

//怎么解决缓存积累问题
//重新设置wnd的大小
//其他策略：视频点播和传文件一样，而视频直播，一旦 ikcp_waitsnd 超过阈值了，除了不再往 KCP 里发送新的数据包，你的视频应该进入一个「丢帧」状态，直到 ikcp_waitsnd 降低到阈值的 1/2，这样你的视频才不会有积累延迟。
//同时，如果你能做的更好点，waitsnd 超过阈值了，代表一段时间内网络传输能力下降了，此时你应该动态降低视频质量，减少码率，等网络恢复了你再恢复。

//协议栈分层组装
//不要试图将任何加密或者 FEC 相关代码实现到 KCP 里面，请实现成不同协议单元并组装成你的协议栈。
//标准协议单元：KCP 的 input/output 方法用来对接下层的 UDP 收发模块。而 ikcp_send、ikcp_recv提供给上层逻辑调用实现协议的收发。
//KCP 的 input/output 方法用来对接下层的 UDP 收发模块。而 ikcp_send、ikcp_recv提供给上层逻辑调用实现协议的收发。

//支持收发可靠和非可靠数据---自己实现
connection.send(channel,pkt,size)
channel == 0 使kcp发送可靠包， channel == 1 使用UDP发送非可靠包
//因为传输是你自己实现的，你可以在发送 UDP 包的头部加一个字节，来代表这个 channel，收到远程来的 UDP 以后，也可以判断 channel == 0 的话，把剩下的数据给 ikcp_input，否则剩下的数据为远程非可靠包
//再统一封装一个 connection.recv 函数，先去 ikcp_recv 那里尝试收包，收不到的话，看刚才有没有收到 channel == 1 的裸 UDP 包，有的话返回给上层用户。

//和现有 TCP 服务器整合
//KCP 可以用在 TCP 的基建上，在登陆时由服务端返回 UDP 端口和密钥，客户端通过 TCP 收到以后，向服务端的 UDP 端口每隔一秒重复发送包含握手信息，直到服务端返回成功或者失败。服务端通过 UDP 传上来的密钥得知该客户端 sockaddr 对应的 TCP 连接，这样就建立 TCP 连接到 UDP 连接的映射关系。为了保持连接和 NAT 出口映射，客户端一般需要每 60 秒就发送一个 UDP 心跳，服务端收到后回复客户端，再在这个 UDP 连接的基础上增加调用 KCP 的逻辑，实现快速可靠传输，这样一套 TCP/UDP 两用的传输系统就建立了。
//客户端连接 TCP，服务端为该 TCP 连接分配一个整数 id 作为标识。
//登录后服务端给客户端发送 UDP 握手信息，包括：自己的 UDP 端口、用户的 TCP 标识 id、32 位随机数 key。
//客户端给服务端 UDP 地址发送握手信息，把刚才服务端发过来的 (id, key) 发送给服务端
//服务端确认 UDP 握手，并且记录该用户 UDP 远端地址
//以后客户端和服务端 UDP 通信，每个包都包含 (id, key)
//服务端用客户端发上来的 (id, key)，确认用户身份，并对比远端地址确认是否是合法用户
//为了保持 NAT 映射关系，UDP 需要每隔 60 秒就像服务器 ping 一次。同时为了防止出口地址改变（NAT 映射改变，或者移动设备切换基站），可以使用重连或者 UDP 重绑定（但是在移动网络环境，出口改变，TCP 也就断了，所以简单重连也没问题）


////Websocket协议 https://zhuanlan.zhihu.com/p/581974844 https://juejin.cn/post/6844903604864679943
//websocket 是一个全双工通信的网络技术，属于应用层协议，
//初始化连接是复用了http的连接通道，底层仍是建立在TCP协议之上的。
//允许服务端主动向客户端推送数据,解决了轮询造成的同步延迟问题
//在WebSocket API中，浏览器和服务器只需要完成一次握手，
//两者之间就直接可以创建持久性的连接，并进行双向数据传输

//需要通过调用WebSocket构造函数来创建一个WebSocket连接，
//构造函数会返回一个WebSocket实例，可以用来监听事件
//WS和WSS分别代表了客户端和服务端之间未加密和加密的通信
var ws = new WebSocket("ws://echo.websocket.org", "myProtocol");

//WebSocket通信
//连接握手分为两个步骤：请求和应答。WebSocket利用了HTTP协议来建立连接，使用的是HTTP的协议升级机制。
//C-->S端
//GET ws://localhost…… HTTP/1.1 ：打开阶段握手，使用http1.1协议。
//Upgrade：websocket，表示请求为特殊http请求，请求的目的是要将客户端和服务端的通信协议从http升级为websocket。
//Sec-websocket-key：Base64 encode 的值，是浏览器随机生成的。客户端向服务端提供的握手信息。
//S-->C端
//101状态码：表示切换协议。服务器根据客户端的请求切换到Websocket协议。
//Sec-websocket-accept: 将请求头中的Set-websocket-key添加字符串并做SHA-1加密后做Base64编码，告知客户端服务器能够发起websocket连接。

//客户端发起连接的约定:
// 如果请求为wss,则在TCP建立后，进行TLS连接建立。
// 请求的方式必须为GET，HTTP版本至少为HTTP1.1。
// 请求头中必须有Host。
// 请求头中必须有Upgrade，取值必须为websocket。
// 请求头中必须有Connection，取值必须为Upgrade。
// 请求头中必须有Sec-WebSocket-Key，取值为16字节随机数的Base64编码。
// 请求头中必须有Sec-WebSocket-Version，取值为13。
// 请求头中可选Sec-WebSocket-Protocol，取值为客户端期望的一个或多个子协议(多个以逗号分割)。
// 请求头中可选Sec-WebSocket-Extensitons，取值为子协议支持的扩展集(一般是压缩方式)。
// 可以包含cookie、Authorization等HTTP规范内合法的请求头。
// 客户端检查服务端的响应:
// 服务端返回状态码为101代表升级成功，否则判定连接失败。
// 响应头中缺少Upgrade或取值不是websocket，判定连接失败。
// 响应头中缺少Connection或取值不是Upgrade，判定连接失败。
// 响应头中缺少Sec-WebSocket-Accept或取值非法（其值为请求头中的Set-websocket-key添加字符串并做SHA-1加密后做Base64编码），判定连接失败。
// 响应头中有Sec-WebSocket-Extensions,但取值不是请求头中的子集，判定连接失败。
// 响应头中有Sec-WebSocket-Protocol,但取值不是请求头中的子集，判定连接失败。
// 服务端处理客户端连接:
// 服务端根据请求中的Sec-WebSocket-Protocol 字段，选择一个子协议返回，如果不返回，表示不同意请求的任何子协议。如果请求中未携带，也不返回。
// 如果建立连接成功，返回状态码为101。
// 响应头Connection设置为Upgrade。
// 响应头Upgrade设置为websocket。
// Sec-WebSocket-Accpet根据请求头Set-websocket-key计算得到，计算方式为：Set-websocket-key的值添加字符串： 258EAFA5-E914-47DA-95CA-C5AB0DC85B11并做SHA-1加密后得到16进制表示的字符串，将每两位当作一个字节进行分隔，得到字节数组，对字节数组做Base64编码。


////RTMP协议 https://mp.weixin.qq.com/s?__biz=MjM5MTkxOTQyMQ==&mid=2257484827&idx=1&sn=249da45e5c2c6bff776fe0fdcff42548&chksm=a5d4e04992a3695f0c9472673dbe22917a78cc3d26c934928c95feb13dc87db3bd62966d8cec&scene=21#wechat_redirect
//Real Time Message Protocol（实时信息传输协议）
//一种应用层的协议，用来解决多媒体数据传输流的多路复用（Multiplexing）和分包（Packetizing）的问题
//RTMP 在两个对等的通信端之间通过可靠的传输协议（例如 TCP）提供双向的消息多路服务，
//用来传输带有时间信息的并行的视频、音频和数据。
//通常的协议的实现会给不同类型的消息赋予不同的优先级，
//当传输能力受到限制时它会影响消息下层流发送的队列顺序

//数据传输流程
//发送端
//YUV、PCM 通过编码 封装成消息message
//把消息分割成块（Chunk）
//将分割后的块（Chunk）通过传输协议（如 TCP）协议发送到网络传输出去
//接收端
//在通过 TCP 协议收到后块（Chunk）数据
//先将块（Chunk）重新组装成消息（Message)
//message 解码 解封装恢复原本的音视频数据

//分包
//RTMP 里有两个重要的概念：消息和块。
//消息，服务于数据封装，是 RTMP 协议中的基本数据单元；块，服务于网络传输
//可以将大的消息（Message）数据分包成小的块（Chunk）通过网络来进行传输，
//这个也是 RTMP 能够实现降低延时的核心原因

//多路复用
//音频、视频数据在分割成块时对传输通道是透明的，
//这样音频、视频数据就能够合到一个传输流中进行同步传输，实现了多路复用。
//这个流就是『块流（Chunk Stream）』。
//在 RTMP 直播中，实时生成视频 Chunk 和音频 Chunk，依次加入到数据流，
//通过网络发送到客户端。
//这样的复用传输流，也是音视频同步的关键

//优先级
//块流（Chunck Stream）这一层没有优先级的划分，
//优先级的设计是放在『消息流（Message Stream）』这层来实现的。
//不同的消息具有不同的优先级。当网络传输能力受限时，
//优先级用来控制消息在网络底层的排队顺序
//控制消息 > 音频消息 > 视频消息 
//要使得这样的优先级能够得到有效执行，分块也是非常关键的措施：
//将大消息切割成小块，可以避免大的低优先级的消息（如视频消息）
//堵塞了发送缓冲从而阻塞了小的高优先级的消息（如音频消息或控制消息）

//块大小协商
//RTMP 发送端，在将消息（Message）切割成块（Chunk）的过程中，
//是以 Chunk Size（默认值 128 字节）为基准进行切割的
//Chunk Size 越大，切割时 CPU 的负担越小；但在带宽不够宽裕的环境下，发送比较耗时，会阻塞其他消息的发送。
//Chunk Size 越小，利于网络发送；但切割时 CPU 的负担越大，而且服务器 CPU 负担也相对较大。不适用于高码率数据流的情况。

//压缩优化
//最完整的 RTMP Chunk Header 长度是 12 字节
//chunk type(2bits)  chunk stream id(6 bits) timestamp(3bytes) message length(3bytes) msg type id(1 byte) msg stream id(4bytes)
//一般情况下，msg stream id 是不会变的，所以针对视频或音频，
//除了第一个 Chunk 的 RTMP Chunk Header 是 12 字节的，
//后续的 Chunk 可省略这个 4 字节的字段，
//采用 8 字节的 RTMP Chunk Heade

//RTMP消息
//消息头 和 有效负载
//消息头:
//消息类型（Message Type），1 字节，表示消息类型。其中 1-6 的取值是保留给协议控制消息使用的。
// 长度（Length），3 字节，表示有效负载的长度（不包含消息头的长度）。单位是字节，使用大端格式。
// 时间戳（Timestamp），4 字节，表示消息时间戳。使用大端格式。
// 消息流 ID（Message Stream ID），3 字节，用来标识消息流。使用大端格式。

//消息交互：
//RTMP 的连接开始于握手。握手内容不同于协议的其它部分，
//它包含三个固定大小的块，而不是带头信息的变长块。
//客户端（发起连接的端点）和服务器各自发送相同的三个块。
//为了演示，这三个块客户端发送的被记做 C0，C1，C2；服务发送的被记做 S0，S1，S2。

//握手顺序：
// 客户端发送 C0 和 C1 块开始握手。
// 客户端必须（MUST）等接收到 S1 后才能发送 C2。
// 客户端必须（MUST）等接收到 S2 后才能发送其它数据。
// 服务器必须（MUST）等接收到 C0 才能发送 S0 和 S1，也可以（MAY）等接到 C1 一起之后。
// 服务器必须（MUST）等到 C1 才能发送 S2。
// 服务器必须（MUST）等到 C2 才能发送其它数据。

//录制视频
//  +--------------------+                     +-----------+
//      |  Publisher Client  |        |            |   Server  |
//      +----------+---------+        |            +-----+-----+
//                 |           Handshaking Done          |
//                 |                  |                  |
//                 |                  |                  |
//        ---+---- |----- Command Message(connect) ----->|
//           |     |                                     |
//           |     |<----- Window Acknowledge Size ------|
//   Connect |     |                                     |
//           |     |<------ Set Peer BandWidth ----------|
//           |     |                                     |
//           |     |------ Window Acknowledge Size ----->|
//           |     |                                     |
//           |     |<----- User Control(StreamBegin) ----|
//           |     |                                     |
//        ---+---- |<-------- Command Message -----------|
//                 |    (_result- connect response)      |
//                 |                                     |
//        ---+---- |--- Command Message(createStream) -->| 
//    Create |     |                                     |
//    Stream |     |                                     |
//        ---+---- |<------- Command Message ------------| 
//                 |  (_result- createStream response)   |
//                 |                                     |
//        ---+---- |---- Command Message(publish) ------>|
//           |     |                                     |
//           |     |<----- User Control(StreamBegin) ----|
//           |     |                                     |
//           |     |------ Data Message (Metadata) ----->|
//           |     |                                     |
// Publishing|     |------------ Audio Data ------------>| 
//   Content |     |                                     |
//           |     |------------ SetChunkSize ---------->|
//           |     |                                     |
//           |     |<--------- Command Message ----------|
//           |     |      (_result- publish result)      |
//           |     |                                     |
//           |     |------------- Video Data ----------->|
//           |     |                  |                  |
//           |     |                  |                  |
//                 |    Until the stream is complete     |
//                 |                  |                  |

//块
//块流的第一个消息都包含了时间戳和载荷的类型识别，
//所以块流除了工作在 RTMP 协议上，
//也可以使用其他协议来发送消息数据。
//RTMP 块流和 RTMP 协议协同工作很适合于各种和样的音视频程序，从一对一和一对多的直播到视频点播服务再到互动会议程序

//格式
//每一个块由块头和数据组成；块头又由块基本头、消息头和扩展时间戳三个部分组成

////基础知识
////TCP/IP如何达到可靠传输的 TCp 传输层 iP是网络层
//超时重传：传输过程丢失或者接收检测出错丢弃
//校验和：TCP将保持它首部和数据的检验和。这是一个端到端的检验和，目的是检测数据在传输过程中的任何变化。如果收到段的检验和有差错，TCP将丢弃这个报文段和不确认收到此报文段
//连续的ARP协议（累积确认/应答）：每个包都有ID，接收方对数据包进行排序，把有序数据传送给应用层
//配合流量和拥塞控制：可变滑动窗口协议 慢开始、拥塞避免、快重传、快恢复
//--为什么三次握手四次挥手
//  三次握手
//  为了防止旧的重复连接初始化造成混乱。
//  同步序列号
//  四次挥手
//  C端发送FIN包，表示客户端不发，还可以接收
//  S端收到FIN，先会回一个ACK，服务端可能还需要处理数据，完毕后再发送FIN给C端，关闭链接
//  TIME_WAIT状态
//  保证TCP协议全双工连接能够可靠关闭
//  保证这次连接的重复数据从网络中消失，如果上次的socket和这次的socket处理的程序一样，就会导致这次连接把上次的数据加进来了
//TCP 的 Keepalive，是由 TCP 层（内核态） 实现的，称为 TCP 保活机制；
//  当客户端和服务端长达一定时间没有进行数据交互时，内核为了确保该连接是否还有效，就会发送探测报文，来检测对方是否还在线，然后来决定是否要关闭该连接

////HTTP协议
//建立在TCP/IP协议上。应用层协议，默认端口号是 80。
//无状态:浏览器对于事务的处理没有记忆能力。举个例子来说就是比如客户请求获得网页之后关闭浏览器，然后再次启动浏览器，登录该网站，但是服务器并不知道客户关闭了一次浏览器。
//keep-alive 用户态，是客户端和服务端的一个约定，如果开启 keep-alive，则服务端在返回 response 后不关闭 TCP 连接；
//以 ASCII 码传输
//规范把 HTTP 请求分为三个部分：状态行、请求头、消息主体
//--HTTP 定义了与服务器交互的不同方法，最基本的方法有4种，
//  分别是GET，POST，PUT，DELETE 查 增 改 删
//  GET: URL中受URL限制 它仅仅是获取资源信息，就像数据库查询一样，不会修改，增加数据，不会影响资源的状态
//  POST: HTTP包内 可能修改变服务器上的资源的请求，会增加种类
//  PUT: 向服务器端发送数据的，从而改变信息，不会增加数据的种类。
//  PATCH: 类似PUT，资源部分的更新
//--Header常见Key
//  Cookie Host Date Connection
//HTTP传输二进制 base64加解密
//HTTPS加密的
//客户端发起ssl请求，服务端返回CA证书
//客户端解析证书验证真伪，获取用于非对称加密的公钥
//利用公钥加密发送对称加密的私钥
//利用对称加密传输数据

////TCP和UDP
// TCP是面向连接且可靠的，UDP是面向非连接且非可靠
// TCP是面向字节流的，UDP是面向报文流
// TCP的传输效率低，UDP传输效率高
// TCP有流量控制，拥塞控制等，UDP没有
// TCP适用于对可靠性要求比较高，但对效率要求低的场景，而UDP适用于对可靠性要求比较低，但对效率要求比较高的场景
// TCP协议应用: HTTP/FTP/TELNET/SMTP UDP协议应用: DNS/SNMP

////输入url地址到页面显示的过程 https://zhuanlan.zhihu.com/p/133906695
//输入地址：可能匹配url 或者缓存 网页显示
//浏览器查找域名ip：
//   解析域名+查看硬盘的hosts文件，看是否有缓存
//   如果没有缓存，浏览器发起DNS请求到本地的DNS服务器，网络接入商（电信）
//   本地DNS服务器递归查询，DNS根服务器 域服务器  DNS 域名到ip的解析
//     DNS负载均衡技术:DNS服务器中为同一个主机名配置多个IP地址,在应答DNS查询时,DNS服务器对每个查询将以DNS文件中主机记录的IP地址按顺序返回不同的解析结果,将客户端的访问引导到不同的机器上去,使得不同的客户端访问不同的服务器,从而达到负载均衡的目的｡
//浏览器向web服务器发送HTTP请求
//   浏览器会以一个随机端口（1024<端口<65535）向服务器的WEB程序（常用的有httpd,nginx等）80端口发起TCP的连接请求
//服务器的永久重定向响应
//   服务器给浏览器响应一个301永久重定向响应
//服务器处理请求---请求发送到了服务器
//   反向代理服务器Nginx
//服务器返回一个 HTTP 响应
//浏览器显示 HTML
//浏览器发送请求获取嵌入在 HTML 中的资源（如图片、音频、视频、CSS、JS等等）


//RTP协议
//（Real-time Transport Protocol，实时传输协议）是一种用于在互联网上传输实时数据的网络协议。
//它通常与RTCP（Real-time Transport Control Protocol，实时传输控制协议）一起使用，用于传输音频和视频等实时多媒体数据。
//RTP将连续的媒体数据流分割成数据包（或称为RTP数据包或RTP数据报），并在网络上传输。
//RTP数据包包含序列号字段，用于标识数据包的顺序和丢失情况。接收方可以利用序列号对数据包进行排序和重组，以便正确地重建原始的媒体流。
//RTP数据包还包含时间戳字段，用于指示数据包对应的媒体数据的时间戳。这对于同步音频和视频流非常重要。
//RTP使用负载类型标识字段来指示RTP数据包中所传输的媒体类型（如音频、视频等），以便接收方能够正确地解析和处理数据。

//将H.264视频流封装到RTP（Real-time Transport Protocol）中涉及一系列步骤和格式转换，其中包括以下几个关键步骤：

//切片（Slice）：H.264视频流通常由一系列帧（Frames）组成，每个帧包含一个或多个切片。在封装过程中，视频帧会被切片成RTP包可以处理的大小。

//RTP头部：为每个RTP数据包添加RTP头部。RTP头部包含序列号、时间戳等信息，用于标识和管理RTP数据包。

//NALU头部：在H.264中，视频数据被分割成NALU（Network Abstraction Layer Units）。在封装过程中，每个NALU的数据前会添加一个RTP特定的头部，用于指示NALU的类型、大小等信息。

//传输：将封装好的RTP数据包通过网络传输到接收端。

//STAP-A用于将多个NALU打包成一个RTP数据包进行传输
//能够在单个RTP数据包中封装多个NALU，从而减少了RTP头部的开销，提高了网络传输的效率。它通常用于传输一些较小的NALU

//单个NALU封装、STAP-A封装、FU-A封装等。在传输H.264视频流时，
//通常会使用FU-A（Fragmentation Unit Type A）封装方式，
//因为它允许将一个NALU分割成多个RTP数据包进行传输，并且适用于大多数场景。

//针对于ipv4地址不够用的情况，我们是如何解决 --- IPV6 NAT 地址重用和地址共享
//ip地址和mac地址的区别都有那些 --- 
    //MAC地址是网络设备在数据链路层中使用的物理地址，用于唯一标识网络设备。6个十六进制数对
    //IP地址是用于在网络中唯一标识和定位设备的一种地址，网络层协议中使用的一种地址。
//转发和重定向 --- 转发：当服务器收到一个请求时，可以将该请求转发给另一个资源进行处理，但是客户端并不知道这个过程。
            //重定向：重定向会告诉客户端，请求的资源已经被移动到另一个位置。客户端接收到重定向响应后，会自动跳转到新的URL去获取资源
//在TCP拥塞控制中，使用了什么样的算法 --- 慢启动：TCP发送方会以指数增长的速率增加发送窗口
            //快重传：当发送方连续收到三个相同的重复确认时，表明有报文段丢失，发送方会立即重传该丢失的报文段，而不必等待超时重传计时器。
            //快恢复：进行快重传后，TCP发送方会将拥塞窗口大小减半，并进入快恢复状态，此时发送方会继续以线性增长的速率增加发送窗口
//TCP粘包 --- 接收方在一次读取数据时，将多个发送方发送的数据包合并成一个或者少于原始数据包数量的现象。TCP是面向流的传输协议
            //数据包中增加长度字段，让接收方根据长度字段来正确解析数据包。使用特殊标记或分隔符来标识数据包的边界
//TCP流量控制的理解 ---- 调整发送方的发送速率，使其不会发送过多数据导致接收方无法及时处理或网络拥塞的机制 滑动窗口大小
            //发送方根据接收方通知的窗口大小来确定发送数据的数量
//TCP协议是如何保证可靠传输 --- 序列号和确认应答、数据校验和重传机制、滑动窗口和流量控制、拥塞控制
//TCP和UDP对应常见的应用层协议有那些 --- TCP: Http SMATP Websocket UDP：KCP\DNS
//DNS域名缓存 --- 减少对远端DNS服务器的查询次数，加快域名解析速度，减轻DNS服务器的负担
            //浏览器缓存\操作系统缓存\路由器缓存\递归DNS服务器缓存(ISP的DNS服务器\根DNS服务器\顶级域（TLD）服务器\权威DNS服务器)
            //结果返回 访问
//Cookie和Session --- Cookie:存储在客户端（通常是用户的浏览器）上的小型数据片 服务端发送 客户端保存 存储用户偏好
            //session:服务器端用来存储信息的机制 存储用户特定的数据，如购物车内容、用户ID等
//HTTP中常用的状态码 --- 1xx：信息性状态码 100 Continue：客户端应继续其请求
            //2xx：成功 200 OK：请求成功，对GET、PUT、PATCH或POST操作的标准响应
            //3xx：重定向： 301 Moved Permanently：请求的页面已永久移动到新位置
            //4xx：客户端错误  400 Bad Request：服务器无法理解请求的格式，客户端不应该再次尝试发送同样的请求
            //5xx：服务器错误  500 Internal Server Error：服务器遇到了一个未曾预料的状况，导致它无法完成对请求的处理
//HTTP 方法 --- GET，POST，PUT，DELETE 查 增 改 删
            //  GET: URL中受URL限制 它仅仅是获取资源信息，就像数据库查询一样，不会修改，增加数据，不会影响资源的状态
            //  POST: HTTP包内 可能修改变服务器上的资源的请求，会增加种类
            //  PUT: 向服务器端发送数据的，从而改变信息，不会增加数据的种类。
            //  PATCH: 类似PUT，资源部分的更新
//TCP三次握手 --- TCP三次握手的核心目的在于双向确认通信双方的接收能力 确保双方都准备好进行数据传输，并且同步序列号
//大端和小端模式  ---- 大端：数据的高字节存储在低地址中，低字节存储在高地址中 
            //Intel的x86架构处理器使用小端模式，而IBM的Power Architecture处理器使用大端模式
//Https加密过程 --- 一种通过TLS/SSL加密通信的HTTP协议，用于安全地传输数据
            //建立连接 服务器返回数字证书 包含服务器的公钥 CA（颁发机构） 数字签名
            //验证证书 客户端会检查证书的签名是否有效
            //协商加密算法 确定使用的加密算法、密钥长度、身份验证方法等 TLS 握手（私钥）
            //生成会话密钥 客户端生成一个随机的会话密钥（Session Key）（利用上面的公钥），用于对称加密通信
            //服务端使用私钥解密客户端发送的会话密钥
//SYN队列 --- TCP协议中的连接建立队列。
            //客户端发送SYN（同步）包给服务器，服务器接收到后回复SYN-ACK（同步-确认）包，
            //最后客户端再回复ACK（确认）包，完成三次握手即建立连接
            //发送FIN包 对方ACK包确认 对方发送FIN包  发送ACK包
//keepalive是什么？如何使用 --- 检测两台计算机之间的连接是否仍然活跃 频繁地发送Keepalive探测包
//connect会阻塞怎么解决 --- 使用的是阻塞式套接字，connect会一直阻塞到连接成功或者发生错误为止
            //使用非阻塞套接字 不要主线程就可以
            //使用异步I/O或事件驱动模型 
//time_wait，close_wait状态产生原因 --- TCP连接处于关闭过程中的两种状态
            //TIME_WAIT状态发生在四次挥手过程的最后阶段  确保对方收到最后一个带有ACK标志的TCP包
            //当一方（如客户端）收到另一方发送的具有FIN标志的TCP包，它会发送一个带有ACK标志的TCP包作为确认，并进入TIME_WAIT状态
            //CLOSE_WAIT状态出现在接收到对方发送的带有FIN标志的TCP包时。
            //当一方（如服务器）收到请求关闭连接的FIN包后，它会发送一个带有ACK标志的TCP包进行确认，并进入CLOSE_WAIT状态。

 
//五种IO模式 --- 阻塞I/O 非阻塞I/O I/O多路复用（事件驱动IO、同时监控多个I/O描述符当任何一个I/O描述符准备好数据时，应用程序就可以对其进行处理）
            //信号驱动  应用程序可以向操作系统注册一个信号处理函数，当数据准备好时，操作系统会发送一个信号，应用程序可以在接收到信号时读取数据
            //异步I/O 
//Socket编程 --- 用TCP/IP协议族中的套接字(Socket)接口，实现客户端和服务器之间的数据交换
            // 创建Socket： Socket是通信的端点，用于标识网络上的一个地址和端口
            // 绑定Socket：服务器程序需要将Socket绑定到特定的IP地址和端口号上，以便客户端能够连接到服务器
            // 监听连接：服务器Socket调用listen()函数开始监听来自客户端的连接请求。
            // 接受连接：服务器Socket调用accept()函数接受客户端的连接请求，与客户端建立连接
            // 发送和接收数据： send recv
            // 关闭连接: close
//select模型和poll模型epoll模型 --- select :监视多个文件描述符直到其中一个文件描述符准备好进行某种IO操作（如读或写）为止。使用select模型的优点是跨平台性好
            //每次调用select都需要遍历所有的文件描述符
            //poll模型:没有最大文件描述符数量的限制
            //epoll模型：以红黑树作为底层的数据结构只有当某个文件描述符准备好进行IO操作时，它才会将这个文件描述符添加到就绪列表中，这避免了遍历所有文件描述符的开销
            //水平触发（LT）和边缘触发（ET）

//虚函数的作用和实现原理，什么是虚函数,有什么作用 -- 它允许在派生类中重写基类的方法，并在运行时根据对象的实际类型来调用相应的方法。虚函数通过动态绑定（也称为运行时多态性）来实现。
//C++多态的实现有那几种？ --- 编译时多态（静态多态）：函数重载  运算符重载 运行时多态（动态多态）：虚函数 抽象类和纯虚函数
//在C++中，对一个对象先malloc后delete这样使用可以吗 -- 构造和析构函数不会被调用 由于malloc和free是对内存的原始操作，容易出现内存泄漏的问题
//C++中右值引用有什么作用 --- 一是对象的移动（Move），二是万能引用（Forwarding Reference）。对于第一种情况，它是为了解决对象的复制效率问题；对于第二种情况，则是为了实现参数的完美传递，避免不必要的拷贝。
            //可以直接将资源（如内存）从一个对象转移到另一个对象
//在C++中，用堆和用栈谁更快一点 --- 栈分配非常快 分配内存仅仅是涉及到移动栈指针的操作。由于栈是线性且连续的内存区域，增加或减少栈空间只需要调整栈顶指针的位置
            //分配堆内存需要在堆的数据结构中寻找足够大的空闲块 内存管理算法

