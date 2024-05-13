// 添加移除视频流水印
//VideoWatermarkNode：实现功能的核心节点
//VideoTeeNode 连接水印pre_watermark_tee_node、post_watermark_tee_node处理的节点工具
//watermark_switch_node：实现两个输入Node的frame选择功能的Node工具


class VideoWatermarkNode{};
class VideoTeeNode{};
class VideoSwitchNode{};

//使用OpenGL实现水印 -- 纹理
//每个顶点就会关联着一个纹理坐标(Texture Coordinate)

//首先会去构造一个ARGB picture
size = width * height * 4;
data = new uint8_t[size];

//创建I420 picture
adata = nullptr;
ydata = nullptr;
udata = nullptr;
vdata = nullptr;

uvdata = nullptr;
auvdata = nullptr;
width = width * 2 / 2;
height = height * 2 / 2;
int area = width *height;
adata = new uint8_t[area];
ydata = new uint8_t[area];
size = this->width * this->height * 5 / 2;
udata = new uint8_t[area / 4];
vdata = new uint8_t[area / 4];

//获取图片数据 最终为BGRA
