
#include "VideoInterface.h"
#include "VideoSource.h"
#ifndef FORLATER_VIDEOMANAGER_H
#define FORLATER_VIDEOMANAGER_H

#endif //FORLATER_VIDEOMANAGER_H


/***
 * 管理所有的VideoSourceInterface
 * 管理设备manager 监听
*/
class VideoManagerIntrface{
public:
    virtual ~VideoManagerIntrface() = default;
    virtual int Set() = 0;
};

class VideoManager : public VideoManagerIntrface{
public:
    VideoManager(const int& ctx){}
    int Set() override{
        video_source_->SetSoucre();
    }
private:
    std::shared_ptr<VideoSourceInterface> video_source_;
    std::shared_ptr<VideoRemoteTrack> remote_track_;
    std::shared_ptr<VideoLocalTrack> remote_track_;

};