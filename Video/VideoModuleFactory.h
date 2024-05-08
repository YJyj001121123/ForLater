#include "VideoDeviceManager.h"
#include "VideoManager.h"
#ifndef FORLATER_VIDEOMODULEFACTORY_H
#define FORLATER_VIDEOMODULEFACTORY_H

#endif //FORLATER_VIDEOMODULEFACTORY_H

//视频设备信息
/***
 * video模块工厂类
 */
class VideoModuleFactory{
    std::shared_ptr<IVideoDeviceManager> CreateVideoDeviceManager() {
        return std::make_shared<VideoDeviceManager>(2);
    }
    std::shared_ptr<VideoManagerIntrface> CreateVideoManager() {
        return std::make_shared<VideoManager>(1);
    }
};


