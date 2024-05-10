//采光不足环境 低端设备采集
//五种算法类型Avlab、Bilateral、Fastcv、Ata、Bmf

/***
 * API
*/
class VideoDenoiserInterface
{
    virtual int setVideoDenoiser() = 0 ;
};

/***
 * node 具有 input和output节点
*/
class VideoDenoiserNodeInterface
{
/***
 * 检测图像合法性
 * 屏幕共享流 不进行处理
 * 判断分辨率是否满足
 * 判断是否超时
 * 在media_thread处理
*/
};


