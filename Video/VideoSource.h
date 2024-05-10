
#include "VideoInterface.h"
#include "VideoPipeline.h"
#include "VideoCapture.h"
#include "VideoDenoiser.h"

/***
 * VideoSourceInterface接口管理视频处理的Pipeline
 * 视频处理的Pipeline是由视频处理不同功能流程的Node组成
 * 其中视频采集的Node为RXVideoCaptureSourceNodeInterface接口对象
*/
class VideoSourceInterface{
public:
    virtual int SetSoucre() = 0;
    virtual int getCameraControl() = 0;
    virtual int EnableHDR() = 0;

    virtual void SetExternalRender(const std::string& render_id, const std::string& stream_id,
        int sink) = 0;
};

class VideoSource : public VideoSourceInterface{
public:
    int SetSoucre() override;
    int getCameraControl() override;
    int EnableHDR() override;
    void SetExternalRender(const std::string& render_id, const std::string& stream_id,
        int sink) override;
private:
    //采集
    std::shared_ptr<VideoInputPipeline> video_pipeline_;
    //外部采集使用的
    std::shared_ptr<VideoExternalCapture> external_capture_;

};