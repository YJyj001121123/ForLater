#include "VideoInterface.h"
#include "VideoManager.h"

#ifndef FORLATER_VIDEODEVICEMANAGER_H
#define FORLATER_VIDEODEVICEMANAGER_H

#endif //FORLATER_VIDEODEVICEMANAGER_H

class VideoDeviceManager: public IVideoDeviceManager{
public:
    VideoDeviceManager(const int& context);
    //实现获取设备信息等
    int setDevice() override{
        video_manager_->Set();
    }
    int getDevice() override;


private:
    std::shared_ptr<VideoManager> video_manager_;
};