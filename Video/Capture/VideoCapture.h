#include "VideoInterface.h"

#ifndef FORLATER_VIDEOCAPTURE_H
#define FORLATER_VIDEOCAPTURE_H

#endif //FORLATER_VIDEOCAPTURE_H
class VideoCaptureInterface{};

/***
 * 视频功能实现的各个平台层实现
*/
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

class VECameraWrapper:public VideoCaptureInterface{
    /***
     * Android/iOS VEcamera 摄像头功能
     */
};

/***
 * 实现外部接口的外部源视频数据接收的实现类
 * 在逻辑上模拟与摄像头采集实现一致对外的视频帧的来源和视频帧推送接口。
 * 当开启视频外部源时，实现对摄像头采集的平台实现的直接替换即开启外部采集
 * 并接收外部视频帧，内部在pipeline中传递处理
*/
class VideoExternalCapture : public VideoCaptureInterface{
/***
 * 外部源
*/
};
