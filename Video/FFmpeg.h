//FFmpeg h264编码
//查找编码器
//设置相关参数（码率、分辨率、帧率、timestamp）
//打开编码器
avcodec_open2()
//对帧数据进行编码
avcodec_send_frame() //将帧送到编码器
avcodec_receive_packet() //从编码器获取编码后的数据包


//init 
avcodec_register_all()
//AVFormatContext  --- 输出上下文
//AVPacket --- 压缩数据包
//AVCodec -- 编码器
//AVFrame
acvodec_find_encoder_by_name()
avcodec_open2()

av_frame_alloc() //初始化设置视频帧格式和分辨率等
av_frame_make_writable() // 确保写入帧
av_frame_get_buffer(AVFrame) //分配buffer
av_image_fill_arrays() //将输入数据 填充到
AVFrame->pts = pts_；

avcodec_send_frame() //编码压缩上面申请到buffer中
