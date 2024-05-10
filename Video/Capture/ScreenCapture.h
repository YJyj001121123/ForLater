//屏幕共享采集 分
//内部采集
//开始采集 停止采集 设置采集区域 设置过滤的窗口 设置期望的编码参数 
//外部采集
// 推送外部屏幕饭帧率 支持设置采集分辨率改变前分辨率
#include "VideoCapture.h"

class ScreenCaptureInterface : public VideoCaptureInterface{

};

class ScreenCapturePC : public ScreenCapture{
    int updateRegion(const Rectangle& regionRect) override;
    int updateExcludedWindows(const std::vector<view_t>& excluded_window_list) override;
};

class ScreenCaptureAndroid : public ScreenCapture{
    int updateRegion(const Rectangle& regionRect) override;
    int updateExcludedWindows(const std::vector<view_t>& excluded_window_list) override;
};

class ScreenCaptureIOS : public ScreenCapture{
    int updateRegion(const Rectangle& regionRect) override;
    int updateExcludedWindows(const std::vector<view_t>& excluded_window_list) override;
};

