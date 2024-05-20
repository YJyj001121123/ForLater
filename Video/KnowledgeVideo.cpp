#include <thread>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <list>
#include <stack>
#include <unordered_set>
#include <queue>
/***
 * YUV Eye
 * 数据格式
 * opengl https://learnopengl-cn.github.io/intro/
 * Android基础：camera、MediaCodec、FFmpeg、
 * Windows多媒体框架：https://blog.csdn.net/jay103/article/details/86665419
 * pipeline 输入、video发送、video接收、输出
 * 全流程
 */
////数据格式
//YUV（YCbCr)格式
//Y（灰度）亮度分量、U（蓝色投影）、V（红色投影）色度分量

//采样：
//色度通道的采样率可以低于亮度通道，而不会显著降低感知质量。
//人眼对 Y 的敏感度远超于对 U 和 V 的敏感
//有时候可以多个 Y 分量共用一组 UV，这样既可以极大得节省空间，又可以不太损失质量
//YUV 格式按照数据大小分为三个格式：
//YUV 420：表示2:1水平取样，垂直2：1采样。 四个Y共用一个U/V值。
//YUV 422：表示2:1水平取样，垂直完全取样。每两个Y共用一对U/V值。
//YUV 444：完全取样。每个像素都有独立的Y/U/V值。
//YUV 411：表示4:1水平取样，垂直完全采样。每四个Y共用一对U/V值，区别是4:1:1是水平方向的4个Y共享一对U/V
//I420：YUV 4:2:0
//并不是说只有U分量，V分量一定为0，而是指U：V互相援引，时见时隐，也就是说对于每一个行，只有一个U或者V分量
//如果一行是4:2:0的话，下一行就是4:0:2，再下一行是4:2:0

////Planar format（平面格式）(YUV 三个分量分开存放)
//width: 图像分辨率的宽
//height: 分辨率高
///I420 YUV420
//先是w * h 长度的 Y
//后面跟 w * h * 0.25 长度的 U
//最后是 w * h * 0.25 长度的 V
//总长度为 w * h * 1.5
Y Y Y Y Y Y
Y Y Y Y Y Y
Y Y Y Y Y Y
Y Y Y Y Y Y
Y Y Y Y Y Y
Y Y Y Y Y Y
U U U
U U U
U U U
V V V
V V V
V V V
///YV12 YUV420
//先是 w * h 长度的 Y
//后面跟 w * h * 0.25 长度的 V
//最后是 w * h * 0.25 长度的 U
//总长度为 w * h * 1.5。
Y Y Y Y Y Y
Y Y Y Y Y Y
Y Y Y Y Y Y
Y Y Y Y Y Y
Y Y Y Y Y Y
Y Y Y Y Y Y
V V V
V V V
V V V
U U U
U U U
U U U
///I422  YUV 422
//先是 w * h 长度的 Y
//后面跟 w * h * 0.5 长度的 U
//最后是 w * h * 0.5 长度的 V
//总长度为 w * h * 2
Y Y Y Y Y Y
Y Y Y Y Y Y
Y Y Y Y Y Y
Y Y Y Y Y Y
Y Y Y Y Y Y
Y Y Y Y Y Y
U U U U U U
U U U U U U
U U U U U U
V V V V V V
V V V V V V
V V V V V V
///YV16 YUV 422
//先是 w * h 长度的 Y
//后面跟 w * h * 0.5 长度的 V
//最后是 w * h * 0.5 长度的 U
//总长度为 w * h * 2
Y Y Y Y Y Y
Y Y Y Y Y Y
Y Y Y Y Y Y
Y Y Y Y Y Y
Y Y Y Y Y Y
Y Y Y Y Y Y
V V V V V V
V V V V V V
V V V V V V
U U U U U U
U U U U U U
U U U U U U
///I444 YUV 444
//先是 w * h 长度的 Y
//后面跟 w * h 长度的 U
//最后是 w * h 长度的 V
//总长度为 w * h * 3
Y Y Y Y Y Y
Y Y Y Y Y Y
Y Y Y Y Y Y
Y Y Y Y Y Y
Y Y Y Y Y Y
Y Y Y Y Y Y
U U U U U U
U U U U U U
U U U U U U
U U U U U U
U U U U U U
U U U U U U
V V V V V V
V V V V V V
V V V V V V
V V V V V V
V V V V V V
V V V V V V
///YV24 YUV 444
//先是 w * h 长度的 Y
//后面跟 w * h 长度的 V
//最后是 w * h 长度的 U
//总长度为 w * h * 3
Y Y Y Y Y Y
Y Y Y Y Y Y
Y Y Y Y Y Y
Y Y Y Y Y Y
Y Y Y Y Y Y
Y Y Y Y Y Y
V V V V V V
V V V V V V
V V V V V V
V V V V V V
V V V V V V
V V V V V V
U U U U U U
U U U U U U
U U U U U U
U U U U U U
U U U U U U
U U U U U U

////Semi-Planar (半平面格式）Y分量单独存放，UV 分量交错存放
///NV12 YUV420
//总长度为 w * h * 1.5
Y Y Y Y Y Y
Y Y Y Y Y Y
Y Y Y Y Y Y
Y Y Y Y Y Y
Y Y Y Y Y Y
Y Y Y Y Y Y
U V U V U V
U V U V U V
U V U V U V
///NV21 YUV420
//总长度为 w * h * 1.5
Y Y Y Y Y Y
Y Y Y Y Y Y
Y Y Y Y Y Y
Y Y Y Y Y Y
Y Y Y Y Y Y
Y Y Y Y Y Y
V U V U V U
V U V U V U
V U V U V U
///NV16 YUV422
//总长度为 w * h * 2
Y Y Y Y Y Y
Y Y Y Y Y Y
Y Y Y Y Y Y
Y Y Y Y Y Y
Y Y Y Y Y Y
Y Y Y Y Y Y
U V U V U V
U V U V U V
U V U V U V
U V U V U V
U V U V U V
U V U V U V
///NV61 YUV422
//总长度为 w * h * 2
Y Y Y Y Y Y
Y Y Y Y Y Y
Y Y Y Y Y Y
Y Y Y Y Y Y
Y Y Y Y Y Y
Y Y Y Y Y Y
V U V U V U
V U V U V U
V U V U V U
V U V U V U
V U V U V U
V U V U V U
///NV24 YUV 444
//总长度为 w * h * 3
Y Y Y Y Y Y
Y Y Y Y Y Y
Y Y Y Y Y Y
Y Y Y Y Y Y
Y Y Y Y Y Y
Y Y Y Y Y Y
U V U V U V U V U V U V
U V U V U V U V U V U V
U V U V U V U V U V U V
U V U V U V U V U V U V
U V U V U V U V U V U V
U V U V U V U V U V U V
///NV42 YUV 444
//总长度为 w * h * 3
Y Y Y Y Y Y
Y Y Y Y Y Y
Y Y Y Y Y Y
Y Y Y Y Y Y
Y Y Y Y Y Y
Y Y Y Y Y Y
V U V U V U V U V U V U
V U V U V U V U V U V U
V U V U V U V U V U V U
V U V U V U V U V U V U
V U V U V U V U V U V U
V U V U V U V U V U V U
////Packed (or Interleaved)(排列交错）
//YUYV YUV 422
Y U V Y   Y U V Y   Y U V Y
Y U V Y   Y U V Y   Y U V Y
Y U V Y   Y U V Y   Y U V Y
Y U V Y   Y U V Y   Y U V Y
Y U V Y   Y U V Y   Y U V Y
Y U V Y   Y U V Y   Y U V Y
//VYUY  YUV 422
V Y U Y   V Y U Y   V Y U Y
V Y U Y   V Y U Y   V Y U Y
V Y U Y   V Y U Y   V Y U Y
V Y U Y   V Y U Y   V Y U Y
V Y U Y   V Y U Y   V Y U Y
V Y U Y   V Y U Y   V Y U Y
//UYVY YUV 422
U Y V Y   U Y V Y   U Y V Y
U Y V Y   U Y V Y   U Y V Y
U Y V Y   U Y V Y   U Y V Y
U Y V Y   U Y V Y   U Y V Y
U Y V Y   U Y V Y   U Y V Y
U Y V Y   U Y V Y   U Y V Y
//YUV 444
Y U V   Y U V   Y U V   Y U V   Y U V   Y U V
Y U V   Y U V   Y U V   Y U V   Y U V   Y U V
Y U V   Y U V   Y U V   Y U V   Y U V   Y U V
Y U V   Y U V   Y U V   Y U V   Y U V   Y U V
Y U V   Y U V   Y U V   Y U V   Y U V   Y U V
Y U V   Y U V   Y U V   Y U V   Y U V   Y U V
////YUV计算
//YUV格式是未经压缩并且只存储了Y/U/V三个通道的数据，
//播放或编码必须要知道分辨率和YUV具体的格式(如nv12/i420/yuvj420ple),
//因此在YUV文件命名时建议名字包含YUV格式名称和分辨率信息

//计算文件大小：
//file size(byte) = width * height * samplateRate * Ceil(bitdepth/8) * frames
//文件大小=宽 * 高 * 采样率 * 向上取整(位深/8) * 帧数
//计算帧数：
//YUV frames = file size(byte) / (width * height * rate * Ceil(bitdepth/8))
//YUV帧数=文件大小/(宽 * 高 * 采样率 * 向上取整(位深/8))

//4 4 4 --->采样率 3
//4 2 2 --->采样率 2
//4 2 0 --->采样率 1.5
//4 1 1 --->采样率 1.5

////OpenGL

////全流程
/***
 * 输入管线
 * ------------采集-----------
 * 内部采集、外部采集、屏幕共享
 * ------------前处理---------
 * 缩放、特效、HDR、降噪、镜像、水印、自定义处理
 * ------------渲染---------
 * 内部渲染、外部渲染
 *
 * 编码管线
 *
 * 发送管线
 *
 * 接收管线
 *
 * 输出管线
 * ------------后处理-----------
 * 超分、锐化
 * ------------渲染---------
 * 内部渲染、外部渲染
 */


/***
 * Input Pipeline 节点 从上至下
 * VideoCaptureSourceNode
 *        |
 *        |
 * VideoFilterNode
 *        |
 *        |
 * VideoInputEntryNode
 *        |
 *        |
 * VideoResizeNode
 *        |
 *        |
 * VideoFrameAbnormalDetcetNode(不正常检查)
 *                      |
 * (Screen pipeline)    | （camera pipeline)
 * VideoSRCAnalysisNode    VideoZoomingNode 
 *       ｜                     ｜
 *       ｜                     ｜
 * VideoDuplicateDetect    VideoEffectNode
 *       ｜                     ｜
 *       ｜                     ｜
 *       ｜                VideoHDRNode
 *       ｜                     ｜
 *       ｜                     ｜
 *       ｜                VideoDenoiseNode
 *       ｜                     ｜
 *       ｜                     ｜
 *       ｜                VideoMirrorNode
 *       ｜                     ｜
 *       ｜                     ｜
 *       ｜                自定义处理流程      FileCaptureSourceNode
 *       ｜                             ｜
 *       ｜                             ｜
 *       ｜                  VideoSwitchNode source_switch_node
 *       ｜                     ｜
 *       ｜                     ｜
 *                  ｜           
 *                  ｜                     
 *                  ｜                    
 *      VideoTeeNode pre_watemark_tee_node
 *          ｜                         ｜
 *          ｜                         ｜
 *   VideoWatermarkNode                |         
 *          ｜                         ｜
 *          ｜                         ｜
 *   VideoTeeNode                      |
     post_watemark_tee_node            |          
 *          ｜                         ｜
 *          ｜                         ｜
 *          |                          |
 *          | --------------   VideoSwitchNode watermark_switch_node 
 *          |                     |
 *          |                     |
 *          |               VideoFilterNode filter_node_after_effect   
 *          |                     |
 *          |                  VideoRenderNode
 *  VidepSEISendNode         
 *          |                     
 *  VideoInputDeliveryNode  
 *          |     
 *          |
*/



/*** Output Pipeline
 * 
 * VideoDeliverNode
 *        |
 *        |
 * VideoAlphaNode
 *        |
 *        |
 * ---------------------------------------------
 *                      |
 *                      | 
 *       ｜                 VideoSuperResolutionNode 
 *       ｜                     ｜
 *       ｜                     ｜
 *       ｜                 VideoSharpenNode
 *       ｜                     ｜
 *       ｜                     ｜
 *       ｜                 VideoTeeNode (video_mos_tee_node)
 *       ｜                     ｜
 *       ｜                     ｜
 *       ｜                     ｜
 *       ｜                     ｜
 *       ｜                     ｜ ------------------
 * --------------------------------------------------------
 *       ｜                     ｜                   |
 *       ｜                     ｜                   |
 *       ｜                     ｜                   |
 * ------------------------------------    VideoFrameAbnormalDetcetNode(不正常检查)
 *                   |                              ｜
 *                   |                              ｜
 *       VideoOutputEntryNode                   VideoMosNode
 *                   |                              
 *                   |                              
 *          VideoFilterNode
 *                   |                              
 *                   |  
 *          VideoRenderNode                            
*/

//在视频编码中，Slice（片）和Frame（帧）
//帧是视频的基本单元，代表一个完整的图像。
//视频流由一系列连续的帧组成，其中包括不同类型的帧，如关键帧（I帧）、预测帧（P帧）、双向参考帧（B帧）等。
//每个帧可以包含一个或多个Slice。

//Slice是在编码帧时使用的一个概念，
//它将帧分成若干个小块进行处理。
//Slice可以看作是帧的一个子集，每个Slice都包含一部分图像数据。
//Slice的使用可以提高并行性和压缩效率，并且有助于实现视频的快速解码和传输。

//IDR帧
//即时解码刷新帧 
//一定是I帧 I帧不一定是IDR帧
//遇到IDR帧，会将参考队列清空，重新开始积累参考帧

//GOP 图像组 
//两个I帧的距离 GOP越大 压缩质量越高

//熵编码 -- h264
//CAVLC 基于上下文的自适应可变长编码
//CABAC 基于上下文的自适应二进制算术编码

//NALU
//VCL Video Code Layer视频编码层
//编码框架去除掉NAL网络抽象层部分 为VCL
//该层产生原始数据比特流SODB(string of Data Bits)
//为了为8的整数倍 补齐“1”和“0” RBSP

//NAL 网络抽象层


//Network Abstraction Layer Unit（网络抽象层单元），是视频编码的基本单元
//NALU Header + RBSP
//每个NALU包含一个完整的视频数据单元，可以是视频帧的一部分或整个视频帧
//主要作用是将视频数据分割成可以独立传输和处理的单元，并且提供了必要的头部信息以及载荷数据

//NALU Header 一个字节

//profile-level 不同画质不同的编码开关
//baseline 视频会议 可视电话 基本画质 
//Extened 流媒体服务   还支持SP帧 和SI帧
    //SP帧是一种空间预测编码帧，它使用空间预测技术来减小图像内部的冗余。SP帧一般作为P帧（Predictive Frame，预测帧）的一种扩展，用于提高视频编码的压缩率。
    //SI帧是一种用于视频编码中的特殊类型的关键帧（I帧），它在传输视频序列时用于关键点的切换。SI帧的主要作用是在视频流中实现更有效的视频切换和编码。
//Main
//Hight

//SPS 
//序列参数集 
//保存编码序列依赖的全局参数，pic_width_mbs_minus1(计算图像宽度)、seq_parameter_set_id(当前编号)等
//PPS
//保存每一帧编码后数据所依赖的参数 entropy_coding_mode_flag（使用的熵编码方式）

//码率控制类型
//CBR 恒定码率
//VBR 可变码率 -- 运动大的帧占比大 
//CVBR 约束可变


//码流分层
// RTP Packet
// NAL Unit   ---    NALU Heder + RBSP
// Slice     ----    Slice Header + Slice Data
// Slice Data ----  Flags....

//RTP打包
//mode 0 一个NALU打包在一个RTP包
//mode 1 打包单个NAL \FU-A
//mdoe 2 一个NALU 多个RTP包


//码流封装格式
//AnnexB
//AVCC
//RTP
    //单一NAL单元模式  单个NAL单元数据必须仅包含一个NAL
    // F 
    // NRI 表示NAL的重要性
    // type

    //STAP-A
    //当NALU长度小，几个NALU组成一个RTC包

    //FU-A 
    //NALU长度超过最大长度， NALU进行分片封包

