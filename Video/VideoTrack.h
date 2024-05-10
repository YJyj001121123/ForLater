#include "VideoPipeline.h"
class VideoRemoteTrackInterface{};

class VideoRemoteTrack : public VideoRemoteTrackInterface{
private:
std::shared_ptr<VideoOutputPipeline> output_pipeline_;
};

class VideoLocalTrack : public VideoRemoteTrackInterface{
private:
std::shared_ptr<VideoSource> video_source_;
};
