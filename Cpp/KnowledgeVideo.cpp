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
 *                      |
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
 *       ｜                       VideoSwitchNode source_switch_node
 *       ｜                     ｜
 *       ｜                     ｜
 *                  ｜           
 *                  ｜                     
 *                  ｜                    
 *      VideoTreeNode pre_watemark_tee_node
 *          ｜                         ｜
 *          ｜                         ｜
 *   VideoWatermarkNode                |         
 *          ｜                         ｜
 *          ｜                         ｜
 *   VideoTreeNode                     |
     post_watemark_tee_node            |          
 *          ｜                         ｜
 *          ｜                         ｜
 *          |                          |
 *          |      -------   VideoSwitchNode watermark_switch_node 
 *          |                     |
 *          |                     |
 *  VidepSEISendNode         VideoFilterNode filter_node_after_effect   
 *          |                     |
 *          |                     |
 *  VideoInputDeliveryNode   VideoRenderNode    
*/






