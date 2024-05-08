#include "VideoInterface.h"
#include "VideoCapture.h"
#ifndef FORLATER_VIDEOPIPELINE_H
#define FORLATER_VIDEOPIPELINE_H

#endif //FORLATER_VIDEOPIPELINE_H




class VideoPipeline{};

class VideoInputPipeline:public VideoPipeline{
public:
    std::shared_ptr<VideoCaptureSourceNodeInterfcae> ideo_capture_source_node_;
};

