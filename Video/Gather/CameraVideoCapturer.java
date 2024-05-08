

public interface  VideoCapture {
     /**
     * 初始化一个camera线程、android应用程序等context，capture的g观察者
     * 如果采集需要传递texture 纹理帧，可以通过surfaceTexture中渲染，SurfaceTextureHelper
     * 作为监听器，进行帧转发到capturerObserver的onFrameCaptured
     * 
     */
    void initialize(SurfaceTextureHelper surfaceTextureHelper, Context applicationContext,
        CapturerObserver capturerObserver);
    /**
     * 其中Surface能对接很多API，包括不限于SurfaceView、SurfaceTexture、SurfaceFlinger、MediaCodec、ImageReader、Camera、Player、EGL，不要懵逼，它本质是一个图像生产者(Image stream Producer)
     * 生产方如何进行拿到Surface灌数据？
     * 通过Surface拿到Canvas（渲染器）
     * 通过Surface创建EGL，OpenGLES --- 游戏 美颜相机
     * 本身有RGB像素数据  --- 软渲染播放器（CPU处理）
     * OpenGLES如何进行关联Surface？---基于EGL
     * EGL衔接gles和本地窗口的桥梁
     * 流程：获取EGLDisplay-->初始化EGL-->获取EGLConfig-->创建渲染表面（surface）-->
     *      创建context-->bind context--绘制（opengles)->交换缓冲区（指向surface）-->释放
     * 理解：创建EGL环境需要绑定一个渲染表面，这个载体可以是系统窗口，也可以是内存的缓冲区，
     *      我们通过这种手段把Surface和EGL进行某种绑定，等gles绘制流程走完再调用swapBuffer后，数据就会写到绑定的那块Surface上
     * SurfacView是特殊的View,它拥有的Surface内容可以直接映射到SurfaceFlinger方的layer，等于是和ViewRoot对应的Surface是平级的
     * 消费方如何处理？
     * 假设你从SurfaceView获取Surface,那么消费方就是SurfaceFlinger（缓冲到屏幕）
     * 假设你从TextureView获取Surface,那么消费方就是SurfaceTexture（缓冲转纹理）
     * 假设你从MediaCodec获取Surface，那么消费方就是MediaCodec（缓冲转编码）
     * 每一套消费方背后都有bufferqueue
     * 总结：
     * 端上图像采集，一般有两种业务场景，摄像头和录屏，
     * 这一步要明白你是消费方，你拿到数据干什么；
     * 想直接拿yuv/rgb裸数据，可以考虑用ImageReader接收；
     * 如果需要图像二次处理，比如美颜和水印，或者剪裁等，建议走GPU(opengles)，
     * 由于GLES接受的是纹理类型，如何把裸数据转成纹理，
     * 两种方式：1、yuv/rgb通过gl转纹理 2、通过SurfaceTexture转，一般会用方式2；
     * 
     * GPU像素拷贝，这种场景也比较常见，如何获取GLES渲染后的数据，一般有三种方式，
     * 1、通过glReadPixels直接或者，
     * 2、通过ImagerReader获取，
     * 3、通过EGLImageKHR+HardwareBuffer获取，一般方式1需要配合pbo效率才好，
     * 纯java建议使用方式2，native层可以使用方式3
     */
    /** 什么是surfaceTexture?
     * Android进行渲染的核心组件。是Surface和OpenGL ES纹理的组合。
     * 用户提供输出到GLES纹理的接口，使GLES可将surface缓冲区作为外部纹理使用
     * 当生产方将新的缓冲区排入bufferqueue时，onFrameAvailable() 回调会通知应用。
     * 然后，应用调用 updateTexImage()，这会释放先前占有的缓冲区，
     * 从队列中获取新缓冲区并执行 EGL 调用，
     * 从而使 GLES 可将此缓冲区作为外部纹理使用。
     * 
     * 1、通过Camera、Video Decoder、OpenGL生成图像流
     * 2、通过Surface入队到BufferQueue，通知GLConsumer
     * 3、GLConsumer从BufferQueue获取图像流GraphicBuffer，并转换为EXTERNAL OES纹理
     * 4、得到OES纹理后，用户方就可以将其转换成普通的纹理，然后应用特效或者上屏
     * 
     * SurfaceTexture使用时，最主要的两个方法：
     * SurfaceTexture (int texName) // 创建SurfaceTextur
     * void updateTexImage () // 将当前图片流更新到纹理
     * 
    */
    void startCapture(int width, int height, int framerate, int min_framerate);
}  

public interface CameraVideoCapturer extends VideoCapturer{
    
}

@SuppressWarnings("deprecation")
abstract class CameraCapturer implements CameraVideoCapturer {
    /**
     * 抽象类：提供相机的基本功能，包括相机会话的创建、启动、停止、切换等。
     * 可以被具体的相机类 camera1 camera2继承扩展
     * CameraEnumerator 枚举设备可用相机设备
     * CameraEventsHandler 相机事件处理
     * UIHandler UI线程上处理相机相关事件
     * ORIENTATION_MODE 相机旋转方式
     */
}





