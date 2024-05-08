#include "VideoInterface.h"
#include "VideoPipeline.h"
#ifndef FORLATER_VIDEOSOURCE_H
#define FORLATER_VIDEOSOURCE_H

#endif //FORLATER_VIDEOSOURCE_H

class VideoSourceInterface{
public:
    virtual int SetSoucre() = 0;
    virtual int getCameraControl() = 0;
    virtual int EnableHDR() = 0;

};

class VideoSource : public VideoSourceInterface{
public:
    int SetSoucre() override;
    int getCameraControl() override;
    int EnableHDR() override;
private:
    std::shared_ptr<VideoInputPipeline> video_pipeline_;
};