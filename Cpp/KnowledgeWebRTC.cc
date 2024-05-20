
//层次 
//   C C++ API   --- 实现P2P的连接
//  Session Mangager  --  管理
// Voice  Video  transport


//Voice 
//iSAC iLBC OPUS Codec  
// Net EQ   音频缓冲的Buffer 网络适配，防止抖动的
// 3A

//Video
//Vp8 codec
//jitter buffer 抖动
//图像算法

//Transport --UDP
//SRTP协议 加密的RTP RTCp
//Multplexing 多流复用一个通道
//p2p（STUN+TURN+ICE)
    //STUN 用简单的UDP穿透NAT,允许应用发现它们与公共互联网之间存在的NAT和防火墙的类型
    //TURN STURN的中继扩展
    //通过修改应用层中的私网地址达到NAT的穿透效果
