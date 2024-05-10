//视频暗光增强


/***
 * API
*/
class VideoHDRInterface
{
    virtual int setLowLightAdjusted() = 0 ;
};

/***
 * node 具有 input和output节点
*/
class VideoHDRNodeInterface
{
/***
 * 检测图像合法性
 * 屏幕共享流 不进行处理
 * 判断分辨率是否满足
 * 判断是否超时
 * 在media_thread处理
*/
};