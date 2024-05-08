
#include "VideoInterface.h"
#include "VideoSource.h"
#ifndef FORLATER_VIDEOMANAGER_H
#define FORLATER_VIDEOMANAGER_H

#endif //FORLATER_VIDEOMANAGER_H

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
};