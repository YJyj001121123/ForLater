//内部渲染 渲染+更新
//外部渲染 视频流与渲染器的绑定

class VideoRenderNode{};
//各端支持的帧格式kOriginal、kI420, RGBA（A是Alpha通道透明度）
//iOS和Mac支持BGRA、NV12；kOriginal指维持原始视频帧格式，以达到效率最大化


struct VideoFrame{
  int64_t timestap;  //视频需要解码时间戳和播放时间戳
  uint32_t width;  //图像的宽
  uint32_t height;
  int rotation;
  int color_space;
  size_t planar;  //帧的平面编号  //3 --- I420   2 --- NV12 NV21 1---RGB
  uint8_t* data;  //平面数据
  size_t stride;

  frame_memory; //分配物理内存


}
//Windows如何渲染
//构造函数 初始化
BITMAPINFO //描述位图信息的结构体
typedef struct tagBITMAPINFO {
  BITMAPINFOHEADER bmiHeader; //位图的尺寸、颜色深度、压缩方式
  RGBQUAD          bmiColors[1];
} BITMAPINFO, *PBITMAPINFO;
//rgb
std::unique_ptr<uint8_t[]> rgb_buffer_;
//i420 
uint8_t* i420_buffer_;
//uint8_t通常用于表示像素数据，因为视频中的像素数据通常是8位灰度值或RGB颜色分量
//视频帧是由一系列像素组成的，每个像素包含了图像的一部分信息，如颜色、亮度等。
//视频帧可以被表示为一个二维数组，其中每个元素是一个像素，而像素的值则可以用uint8_t类型来表示。

//allocate_video_buffer --- init时候去调用
//分配内存用于存储视频帧数据

//render_frame()
//渲染视频帧
//视频帧的宽度、高度以及旋转模式等信息
//函数根据视频帧的像素格式，将像素数据转换为RGB格式。
//如果视频帧的像素格式是I420或NV12，则需要先进行色彩空间转换，将其转换为RGB格式。
//绘制到GDI

//首先获取源数据到宽、高
int32_t dst_w = VideoFrame.width();
int32_t dst_h = VideoFrame.height();
//如果需要旋转 交换宽高就可以
//定义指针指向I420格式的YUV分量
uint8_t* dst_y = VideoFrame; //特定对齐方式申请的内存
uint8_t* dst_u = dst_y + dst_w * dst_h;
uint8_t* dst_v = dst_u + dst_w * dst_h / 4;
//定义跨度--内存对齐需要
//跨度（stride）是指在内存中表示每个分量的一行数据所占用的字节数。
//它不仅仅取决于图像的宽度，还受到内存对齐、缓存行对齐等因素的影响
//在常见的YUV420格式中，U和V分量的宽度和高度通常是Y分量的一半
int32_t dst_stride_y = dst_w;
int32_t dst_stride_u = dst_w / 2;
int32_t dst_stride_v = dst_w / 2;
//转换数据为RGB
//设置窗口
void* windows;
HDC hdc = GetDC((HWND)window); // 获取句柄
RECT rc;
GetClientRect((HWND)window, &rc);  //区域
HDC dc_mem = ::CreateCompatibleDC(hdc); //一个与窗口设备ctx兼容的内存设备ctx
SetStretchBltMode(dc_mem, HALFTONE);

HBITMAP bmp_mem = ::CreateCompatibleBitmap(hdc, rc.right, rc.bottom);
HGDIOBJ bmp_old = ::SelectObject(dc_mem, bmp_mem); //创建与windows兼容的位图

HBRUSH brush = ::CreateSolidBrush(RGB(0, 0, 0));  
RECT logical_rect = {0, 0, logical_area.x, logical_area.y};
FillRect(dc_mem, &logical_rect, brush);
DeleteObject(brush);  //创建画刷

//计算位置

StretchDIBit() //镜像模式选择合适的绘制函数，绘制RGB格式的视频帧到dc_mem
BitBlt()//将dc_mem中的图像复制到窗口的设备ctx hdc中


//iOS/Android
//gl渲染
//创建一个glcontext 同时需要SurfaceView/TextureView
//加载视频数据 
//创建Open GL的着色器和纹理 --创建着色器来定义渲染的效果，以及纹理来存储视频帧的像素数据。
    //通常，你会创建两个着色器：一个用于顶点处理，另一个用于片段处理。
    //然后，你需要将视频帧的像素数据加载到纹理中
//渲染视频帧---Looper 
  //从视频源获取视频帧，并将其绘制到 OpenGL 的纹理中。然后，你可以使用着色器将纹理渲染到屏幕上
  //过程通常包括将视频帧数据复制到纹理、绑定纹理到 OpenGL 的纹理单元、设置着色器属性等操作
//处理屏幕大小变化
  //OpenGL 视图和渲染器的大小，并重新计算投影矩阵等参数
//释放资源

//renderframe
//丢帧逻辑---taskqueue 有任务 放弃当前或者替换旧的
//异步渲染任务
  //记录视频帧离开解码器或外部输入的时间
  //获取渲染开始时的时间点（毫秒）和上传纹理开始时的时间点（纳秒），用于后续计算渲染过程中的耗时
  //根据视频帧的内存类型进行不同渲染操作
    //Windows需要准备渲染帧--准备窗口 HWND
    //Linux需要准备渲染帧
XWindowAttributes attr
    //如果是Byte内存--内存帧渲染
    //如果是Texture视频帧数据已经在OpenGL的纹理中了---OpenGL渲染
    //CVPixel视频帧数据是 Core Video 像素缓冲区
      //Mac 无法操作，转为I420--内存帧渲染
      //其他就正常渲染
    //D3D11Textur 视频帧数据已经上传到 DirectX 11 纹理中--- d3d帧渲染
    //NvidiaJetsonDma，表示视频帧数据是 NVIDIA Jetson 的 DMA 内存类型
      //转为I420 ---内存渲染
//准备渲染帧


//具体渲染
//I420 
  //检测窗口的尺寸
  //保证OpenGL CTX存活
  //YUV格式的内存数据上传到纹理中
  //计算最终的纹理矩阵 根据给定的旋转角度和原始纹理矩阵来应用镜像、缩放和旋转变换
  //计算最终的顶点矩阵，根据给定的帧宽度、帧高度和旋转角度来应用镜像、缩放和旋转变换
  //获取渲染的缩放滤波器类型
  //开始绘制
    //准备着色器
    //绑定着色器

enum corlorSpace{
  //BT.601是一种视频颜色空间标准，用于定义数字视频信号的编码方式和颜色表示
  //颜色范围受限16到235
  //使用可以提高图像质量，因为它使得黑色和白色之外的颜色能够更好地区分开来，
  //从而减少了颜色量化误差和显示设备的不适配问题
  //使用（0-255）范围的主要优势在于它提供了更丰富的颜色细节和更大的动态范围。
  //这对于一些高要求的应用场景，如图像处理、色彩分析和特效处理等非常重要
  YCbCrBT601;   //标清
  //为了兼容性和节省带宽
  YCbCrBT709;   //高清
  //更广的色域范围和更高的色度精度，可实现更丰富、更真实的色彩表现
  YCbCrBT2020；
}
enum FilterType {
   kOrigin,  
   kBox,   //盒滤波
   kBilinear,  //双线性
   kBicubic,   //双三次滤波
   kLanczos };

//准备着色器
//判断 YUV OES RGB（2D纹理）BGR RGB_RECTANGLE（指示某些图像数据采用了矩形纹理（Texture Rectangle）的存储方式）
//如果是YUV 根据corlorSpace + 滤波类型判断 --给出的是片段着色器的代码
//预定义的着色器代码的开头部分，
//后一个字符串则可能是针对特定的 YUV 格式和色彩空间的着色器代码的结尾部分

//利用顶点着色器代码和片段着色器的代码 初始化shader
//bind 程序
//加载 uniform 变量的位置信息。根据不同的着色器类型（type），设置不同的 uniform 变量。
  //uniform 变量，用于从 OpenGL 应用程序代码向着色器代码发送数据
  //例如，如果着色器类型是 kYuv，则发送 y_tex、u_tex 和 v_tex 的位置信息；
  //如果类型是 kRGB、kBGR 或 kRGB_RECTANGLE，则发送 rgb_tex 的位置信息；
  //如果是 kOes，则发送 oes_tex 的位置信息；如果是 kRgb2Yuv 或 kOes2Yuv，则发送 rgb2yuv 和 yuvOffset 的位置信息
//初始化顶点着色器属性 in_pos in_tc 位置属性和纹理属性

static const char VERTEX_SHADER_STRING[] =
                "varying vec2 interp_tc;\n" //用于在顶点着色器和片元着色器之间传递数据。这里声明的是一个二维向量类型
                "attribute vec4 in_pos;\n"  //接收顶点的位置信息。这里声明的是一个四维向量类型，通常用于表示顶点的坐标位置
                "attribute vec4 in_tc;\n" //接收顶点的纹理坐标信息。这里声明的是一个四维向量类型，通常用于表示纹理坐标
                "\n"
                "uniform mat4 texMatrix;\n" //接收纹理坐标的变换矩阵。这里声明的是一个四维矩阵类型
                "uniform mat4 projection;\n" //接收投影矩阵。这里声明的是一个四维矩阵类型
                "\n"
                "void main() {\n"
                "    gl_Position = projection * in_pos;\n" //计算顶点的最终位置，使用投影矩阵将顶点的位置信息进行变换，并将结果赋值给内置变量 gl_Position，表示顶点的裁剪空间坐标。
                "    interp_tc = (texMatrix * in_tc).xy;\n" //计算顶点的最终纹理坐标，将顶点的纹理坐标进行变换，并将结果赋值给 interp_tc 变量，以便在片元着色器中进行插值处理。
                "}\n";

//EAGL iOS --限制在主线程
//创建EAGL ctx
EAGLContext *context = [[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2];
//EAGLLayer 与 EAGL 上下文协作显示图形内容
CAEAGLLayer *eaglLayer = (CAEAGLLayer *)view.layer;
//创建的 EAGL 上下文设置为当前上下文，这样 OpenGL ES 才能正确地与其交互
[EAGLContext setCurrentContext:context];
