#include <thread>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <list>
#include <stack>
#include <unordered_set>
#include <queue>

#ifndef FORLATER_VIDEOINTERFACE_H
#define FORLATER_VIDEOINTERFACE_H

#endif //FORLATER_VIDEOINTERFACE_H

//用户调用开始采集
//VideoCaptureInterface实现的类 去进行采集
//采集实例将图像回调给了VideoCaptureSoureceNodeInterface
//采集实例将图像回调给了VideoCaptureSoureceNodeInterface通过一个OutPut节点
//输入到前处理流程 在推送发送pipeline进行编码推流

//用户调用推送外部源
//传递给VideoExternalCapture实例
//采集实例将图像回调给了VideoCaptureSoureceNodeInterface
//采集实例将图像回调给了VideoCaptureSoureceNodeInterface通过一个OutPut节点
//输入到前处理流程 在推送发送pipeline进行编码推流


/***
 * 接口类
*/
class IVideoDeviceManager{
    IVideoDeviceManager(){};
    virtual int setDevice() = 0;
    virtual int getDevice() = 0;
};
class IVideoCapture{
    virtual int startVideoCapture()=0;
    virtual int switchCamera(int camera_id) = 0;
 //   ...
};

class ICameraControl{
    
};

class IVideoFrame{};
class IEncodedVideoFrame{};
class IVideoCaptureExternal{
    virtual int setVideoSourceType(
                    int stream_index, int source_type) = 0;
    virtual int pushExternalVideoFrame(IVideoFrame* frame) = 0;//推送外部视频帧
    virtual int pushExternalEncodedVideoFrame(int index,
                        int video_index, IEncodedVideoFrame* video_stream)=0;
                        //推送自定义编码后的视频流
};