#include "VideoInterface.h"

#ifndef FORLATER_VIDEOCAPTURE_H
#define FORLATER_VIDEOCAPTURE_H

#endif //FORLATER_VIDEOCAPTURE_H
class VideoCaptureInterface{};

class VideoAndroidCapture:public VideoCaptureInterface{
    /***
     * C++调用JAVA jni
     * 需要VECamera Wraper
     */
};

class VideoAppleCapture:public VideoCaptureInterface{
    /***
     * OC实现 混编
     * iOS需要VECamera Wraper
     * AVCaptureSession
     */
};

class VideoLinuxCapture:public VideoCaptureInterface{
    /***
     * C++
     * V4L2 USB摄像头获取视频帧
     */
};

class VideoWindowsCapture:public VideoCaptureInterface{
    /***
     * C++
     */
};

class VideoCaptureSourceNodeInterfcae{ };
class VideoCaptureSourceNode:public VideoCaptureSourceNodeInterfcae {
private:
    std::shared_ptr<VideoCaptureInterface> video_Capture_;
};

