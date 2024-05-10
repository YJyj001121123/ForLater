#include "VideoInterface.h"
#include "VideoPipeline.h"
#include "VideoCapture.h"

#ifndef FORLATER_VIDEOSOURCE_H
#define FORLATER_VIDEOSOURCE_H

#endif //FORLATER_VIDEOSOURCE_H


/***
 * 实现视频采集功能的业务逻辑，依赖视频功能实现的各个平台层实现
*/
class VideoCaptureSoureceNodeInterface : public ICameraControl{
        //virtual int xx () = 0;
};

class VideoCaptureSourceNode : public VideoCaptureSoureceNodeInterface{
private:
std::shared_ptr<VideoCaptureInterface> video_capture_;
std::shared_ptr<VideoCaptureFactory> capture_factory_; //区分camera采集、screen capture、file capture
};