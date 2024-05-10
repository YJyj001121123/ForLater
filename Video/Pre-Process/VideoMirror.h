//本端视频流开启镜像，控制采集视频帧渲染镜像，控制发送编码视频帧镜像
//修改远端视频帧渲染镜像模式

//渲染镜像影响本机画面
//编码镜像影响订阅端画面

class VideoMirrorInterface
{
virtual int setLocalVideoMirrorType(int mirror_type) = 0;
virtual int setRemoteVideoMirrorType(int mirror_type) = 0;

};
//本端处理
//设置：VideoInputEntry的mirrod_mode 
//生效：Mirror node 处理
class VideoMirrorNodeInterface{

};
//远端处理 渲染镜像模式
//VideoManger 管理多个VideoRemoteTrack (stream_id)区分
//每一个VideoRemoteTrack 对应一个下行的outputpipeline