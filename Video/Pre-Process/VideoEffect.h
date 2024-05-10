

//开启需要初始化openGL环境
//设置模型和资源的路径

//视频帧（NV12、I420、Textture） --->纹理格式帧--->人脸识别--->美颜--->纹理格式帧转rgba--->
//rgba转I420--->编码发送



class VideoEffectNode{
    private:
    std::shared_ptr<VideoEffect> video_effect_;
};

class VideoEffect{
    private:
    std::shared_ptr<EFLooper> ef_looper_;
    std::shared_ptr<EGLContextHelper> egl_context_helper_;
    std::shared_ptr<EGLRender> egl_render_;
    std::shared_ptr<BasicBeauty> basic_beauty_;
    std::shared_ptr<Resourc> resource_;

}
