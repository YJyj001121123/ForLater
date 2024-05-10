#include "VideoInterface.h"
#include "VideoCapture.h"
#include "VideoCaptureSourceNode.h"
#include "VideoDenoiser.h"
#include "VideoHDR.h"
#include "VideoMirror.h"
#include "VideoWatermark.h"
#include "VideoEffect.h"
#include "VideoRender.h"

#ifndef FORLATER_VIDEOPIPELINE_H
#define FORLATER_VIDEOPIPELINE_H

#endif //FORLATER_VIDEOPIPELINE_H




class VideoPipeline{};

class VideoInputPipelineInterface{};

class VideoInputPipeline : public VideoInputPipelineInterface, public VideoPipeline{
public:    
    std::shared_ptr<VideoCaptureSourceNodeInterfcae> video_capture_source_node_;
    //前处理
    std::shared_ptr<VideoDenoiserNodeInterface> video_denoiser_;
    std::shared_ptr<VideoHDRNodeInterface> video_denoiser_;
    std::shared_ptr<VideoMirrorNodeInterface> video_denoiser_;
    std::shared_ptr<VideoEffectNode> video_effect_;
    std::shared_ptr<VideoTeeNode> video_tee_;
    std::shared_ptr<VideoWatermarkNode> video_watermark_;
    std::shared_ptr<VideoSwitchNode> video_watermark_switch;
    //渲染
    std::shared_ptr<VideoRenderNode> video_render_; //external






};

class VideoOutputPipelineInterface {};
class VideoOutputPipeline: public VideoOutputPipelineInterface, public VideoPipeline{
public:  
};  

