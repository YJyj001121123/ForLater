# Just casually browsing with a bad mindset.

# Android 音视频
## 三种方式绘制图片
1、imageView
```java
Bitmap bitmap = BitmapFactory.decodeFile(Environment.getExternalStorageDirectory().getPath() + File.separator + "11.jpg"); // 解码生成bitmap
imageView.setImageBitmap(bitmap);
```

2、SurfaceView
```java
SurfaceView surfaceView = (SurfaceView) findViewById(R.id.surface); //获取surfaceView对象
surfaceView.getHolder().addCallback(new SurfaceHolder.Callback() {
    @Override
    public void surfaceCreated(SurfaceHolder surfaceHolder) {

        if (surfaceHolder == null) {
            return;
        }

        Paint paint = new Paint();
        paint.setAntiAlias(true); //设置抗锯齿和画笔样式。
        paint.setStyle(Paint.Style.STROKE);

        Bitmap bitmap = BitmapFactory.decodeFile(Environment.getExternalStorageDirectory().getPath() + File.separator + "11.jpg");  // 获取bitmap
        Canvas canvas = surfaceHolder.lockCanvas();  // 先锁定当前surfaceView的画布
        canvas.drawBitmap(bitmap, 0, 0, paint); //执行绘制操作
        surfaceHolder.unlockCanvasAndPost(canvas); // 解除锁定并显示在界面上
    }

    @Override
    public void surfaceChanged(SurfaceHolder surfaceHolder, int i, int i1, int i2) {

    }

    @Override
    public void surfaceDestroyed(SurfaceHolder surfaceHolder) {

    }
});
```

3、自定义View
```java
public class CustomView extends View {

    Paint paint = new Paint();
    Bitmap bitmap;

    public CustomView(Context context) {
        super(context);
        paint.setAntiAlias(true);
        paint.setStyle(Paint.Style.STROKE);
        bitmap = BitmapFactory.decodeFile(Environment.getExternalStorageDirectory().getPath() + File.separator + "11.jpg");  // 获取bitmap
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);

        // 不建议在onDraw做任何分配内存的操作
        if (bitmap != null) {
            canvas.drawBitmap(bitmap, 0, 0, paint);
        }
    }
}
```

## AudioRecord 采集音频PCM
1、构造一个AudioRecord对象，其中需要的最小录音缓存buffer大小可以通过getMinBufferSize方法得到。如果buffer容量过小，将导致对象构造的失败。
2、初始化一个buffer，该buffer大于等于AudioRecord对象用于写声音数据的buffer大小。
3、开始录音
4、创建一个数据流，一边从AudioRecord中读取声音数据到初始化的buffer，一边将buffer中数据导入数据流。
5、关闭数据流
6、停止录音

```java
private AudioRecord audioRecord = null;  // 声明 AudioRecord 对象
private int recordBufSize = 0; // 声明recoordBufffer的大小字段

public void createAudioRecord() {
　　recordBufSize = AudioRecord.getMinBufferSize(frequency, channelConfiguration, EncodingBitRate);  //audioRecord能接受的最小的buffer大小
   audioRecord = new AudioRecord(MediaRecorder.AudioSource.MIC, frequency, channelConfiguration, EncodingBitRate, recordBufSize);
} //MIC 录制源 

byte data[] = new byte[recordBufSize];

audioRecord.startRecording();
isRecording = true;

FileOutputStream os = null;
try {
    os = new FileOutputStream(filename);
} catch (FileNotFoundException e) {
    e.printStackTrace();
}
if (null != os) {
    while (isRecording) {
        //从设备采集数据了，存储到data
        read = audioRecord.read(data, 0, recordBufSize);
　　　　  // 如果读取音频数据没有出现错误，就将数据写入到文件
        if (AudioRecord.ERROR_INVALID_OPERATION != read) {
            try {
                os.write(data);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    try {
        os.close();
    } catch (IOException e) {
        e.printStackTrace();
    }
} 

if (null != audioRecord) {
　　audioRecord.stop();
   audioRecord.release();
　　audioRecord = null;
   recordingThread = null;
}
```

注意：现在的文件里面的内容仅仅是最原始的音频数据，为raw
4、如何播放？
在文件的数据开头加入WAVE HEAD数据即可，也就是文件头。

Android SDK 提供了两套音频采集的API，分别是：MediaRecorder 和 AudioRecord，前者是一个更加上层一点的API，它可以直接把手机麦克风录入的音频数据进行编码压缩（如AMR、MP3等）并存成文件，而后者则更接近底层，能够更加自由灵活地控制，可以得到原始的一帧帧PCM音频数据。如果想简单地做一个录音机，录制成音频文件，则推荐使用 MediaRecorder，而如果需要对音频做进一步的算法处理、或者采用第三方的编码库进行压缩、以及网络传输等应用，则建议使用 AudioRecord，其实 MediaRecorder 底层也是调用了 AudioRecord 与 Android Framework 层的 AudioFlinger 进行交互的。直播中实时采集音频自然是要用AudioRecord了。

## AudioTrack 播放PCM
1、MODE_STREAM
通过write一次次把音频数据写到AudioTrack中 需要定时器
2、MODE_STATIC
play之前把所有数据通过一次write调用传递到AudioTrack中的内部缓冲区，后续就不必再传递数据了。
```java
private static AudioTrack createAudioTrack(int sampleRate) {
        AudioTrack audioTrack;
        int bufferSize = AudioTrack.getMinBufferSize(sampleRate, AudioFormat.CHANNEL_OUT_MONO, AudioFormat.ENCODING_PCM_16BIT);
        if (Build.VERSION.SDK_INT < Build.VERSION_CODES.M) {
            audioTrack = new AudioTrack(AudioManager.STREAM_MUSIC
                    , sampleRate
                    , AudioFormat.CHANNEL_OUT_MONO
                    , AudioFormat.ENCODING_PCM_32BIT
                    , bufferSize
                    , AudioTrack.MODE_STREAM);
        } else {
            AudioAttributes audioAttributes = new AudioAttributes.Builder()
                    .setUsage(AudioAttributes.USAGE_MEDIA)
                    .setContentType(AudioAttributes.CONTENT_TYPE_MUSIC)
                    .build();
            AudioFormat audioFormat = new AudioFormat.Builder()
                    .setSampleRate(sampleRate)
                    .setChannelMask(AudioFormat.CHANNEL_OUT_MONO)
                    .setEncoding(AudioFormat.ENCODING_PCM_16BIT)
                    .build();
            audioTrack = new AudioTrack(audioAttributes, audioFormat, bufferSize,
                    AudioTrack.MODE_STREAM, AudioManager.AUDIO_SESSION_ID_GENERATE);
        }
        return audioTrack;
    }
```
Frame是一个单位，用来描述数据量的多少。1单位的Frame等于1个采样点的字节数×声道数（比如PCM16，双声道的1个Frame等于2×2=4字节）

MediaPlayer可以播放多种格式的声音文件，例如MP3，AAC，WAV，OGG，MIDI等。MediaPlayer会在framework层创建对应的音频解码器。而AudioTrack只能播放已经解码的PCM流，如果对比支持的文件格式的话则是AudioTrack只支持wav格式的音频文件，因为wav格式的音频文件大部分都是PCM流。AudioTrack不创建解码器，所以只能播放不需要解码的wav文件。

## camera API采集
使用 SurfaceView、TextureView 来预览 Camera 数据，取到 NV21 的数据回调
``` java
SurfaceView surfaceView;
TextureView textureView;

Camera camera;
@Override
protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_main);

    surfaceView = (SurfaceView) findViewById(R.id.surface_view);
    surfaceView.getHolder().addCallback(this);

    // 打开摄像头并将展示方向旋转90度
    camera = Camera.open();
    camera.setDisplayOrientation(90);
}
//------ Surface 预览 -------
@Override
public void surfaceCreated(SurfaceHolder surfaceHolder) {
    try {
        camera.setPreviewDisplay(surfaceHolder);
        camera.startPreview();
    } catch (IOException e) {
        e.printStackTrace();
    }
}
@Override
public void surfaceChanged(SurfaceHolder surfaceHolder, int format, int w, int h) {
}

@Override
public void surfaceDestroyed(SurfaceHolder surfaceHolder){
    camera.release();
}

@Override
protected void onCreate(Bundle savedInstanceState) {
    super.onCreate(savedInstanceState);
    setContentView(R.layout.activity_main);
    textureView = (TextureView) findViewById(R.id.texture_view);
    textureView.setSurfaceTextureListener(this);
    // 打开摄像头并将展示方向旋转90度
    camera = Camera.open();
    camera.setDisplayOrientation(90);
}　　//------ Texture 预览 -------
@Override
public void onSurfaceTextureAvailable(SurfaceTexture surfaceTexture, int i, int i1) {
    try {
        camera.setPreviewTexture(surfaceTexture);
        camera.startPreview();
    } catch (IOException e) {
        e.printStackTrace();
    }
}

@Override
public void onSurfaceTextureSizeChanged(SurfaceTexture surfaceTexture, int i, int i1) {

}

@Override
public boolean onSurfaceTextureDestroyed(SurfaceTexture surfaceTexture) {
    camera.release();
    return false;
}

@Override
public void onSurfaceTextureUpdated(SurfaceTexture surfaceTexture) {

}
```

## MediaExtractor、MediaMuxer
1、MediaExtractor --- 音视频数据分离
主要API：
setDataSource(String path)：即可以设置本地文件又可以设置网络文件
getTrackCount()：得到源文件通道数
getTrackFormat(int index)：获取指定（index）的通道格式
getSampleTime()：返回当前的时间戳
readSampleData(ByteBuffer byteBuf, int offset)：把指定通道中的数据按偏移量读取到ByteBuffer中；
advance()：读取下一帧数据
release(): 读取结束后释放资源
```java
MediaExtractor extractor = new MediaExtractor();
extractor.setDataSource(...);
int numTracks = extractor.getTrackCount();
//遍历轨道
for (int i = 0; i < numTracks; ++i) {
  MediaFormat format = extractor.getTrackFormat(i);
  String mime = format.getString(MediaFormat.KEY_MIME);
  if (weAreInterestedInThisTrack) {
    extractor.selectTrack(i);
  }
}
//使用 readSampleData() 方法读取样本数据到指定的 ByteBuffer 中，这里使用一个循环来持续读取数据直到结束
ByteBuffer inputBuffer = ByteBuffer.allocate(...)
while (extractor.readSampleData(inputBuffer, ...) >= 0) {
  int trackIndex = extractor.getSampleTrackIndex();
  long presentationTimeUs = extractor.getSampleTime();
  ...
  extractor.advance();
}
extractor.release();
extractor = null;
```
2、MediaMuxer --- 音频与视频混合成一个音视频文件、生成音频或视频文件
主要API：
MediaMuxer(String path, int format)：path:输出文件的名称 format:输出文件的格式；当前只支持MP4格式；
addTrack(MediaFormat format)：添加通道；我们更多的是使用MediaCodec.getOutpurForma()或Extractor.getTrackFormat(int index)来获取MediaFormat;也可以自己创建；
start()：开始合成文件
writeSampleData(int trackIndex, ByteBuffer byteBuf, MediaCodec.BufferInfo bufferInfo)：把ByteBuffer中的数据写入到在构造器设置的文件中；
stop()：停止合成文件
release()：释放资源
```java
MediaMuxer muxer = new MediaMuxer("temp.mp4", OutputFormat.MUXER_OUTPUT_MPEG_4);
 // More often, the MediaFormat will be retrieved from MediaCodec.getOutputFormat()
 // or MediaExtractor.getTrackFormat().
 MediaFormat audioFormat = new MediaFormat(...);
 MediaFormat videoFormat = new MediaFormat(...);
 //添加轨道
 int audioTrackIndex = muxer.addTrack(audioFormat);
 int videoTrackIndex = muxer.addTrack(videoFormat);
 ByteBuffer inputBuffer = ByteBuffer.allocate(bufferSize);
 boolean finished = false;
 BufferInfo bufferInfo = new BufferInfo();

 muxer.start();
 //循环写入数据getInputBuffer() 方法会将编码后的音视频数据填充到指定的 ByteBuffer 中，然后通过 writeSampleData() 方法将数据写入到指定轨道的混合器中。
 while(!finished) {
   // getInputBuffer() will fill the inputBuffer with one frame of encoded
   // sample from either MediaCodec or MediaExtractor, set isAudioSample to
   // true when the sample is audio data, set up all the fields of bufferInfo,
   // and return true if there are no more samples.
   finished = getInputBuffer(inputBuffer, isAudioSample, bufferInfo);
   if (!finished) {
     int currentTrackIndex = isAudioSample ? audioTrackIndex : videoTrackIndex;
     muxer.writeSampleData(currentTrackIndex, inputBuffer, bufferInfo);
   }
 };
 muxer.stop();
 muxer.release();
```

## MediaCodec API
用于使用一些基本的多媒体编解码器（音视频编解码组件），它是Android基本的多媒体支持基础架构的一部分通常和 MediaExtractor, MediaSync, MediaMuxer, MediaCrypto, MediaDrm, Image, Surface, and AudioTrack 一起使用。

编解码器能处理的数据类型为： 压缩数据、原始音频数据和原始视频数据。你可以通过ByteBuffers能够处理这三种数据，但是需要你提供一个Surface，用于对原始的视频数据进行展示，这样也能提高编解码的性能。Surface使用的是本地的视频缓冲区，这个缓冲区不映射或拷贝到ByteBuffers。这样的机制让编解码器的效率更高。通常在使用Surface的时候，无法访问原始的视频数据，但是你可以使用ImageReader访问解码后的原始视频帧。在使用ByteBuffer的模式下，您可以使用Image类和getInput/OutputImage（int）访问原始视频帧。

1、生命周期 -- Stopped、Executing、Released。
Stopped的状态下也分为三种子状态：Uninitialized、Configured、Error。
Executing的状态下也分为三种子状态：Flushed, Running、End-of-Stream。
2、MediaCodec可以处理具体的视频流，主要有这几个方法：
getInputBuffers：获取需要编码数据的输入流队列，返回的是一个ByteBuffer数组
queueInputBuffer：输入流入队列
dequeueInputBuffer：从输入流队列中取数据进行编码操作
getOutputBuffers：获取编解码之后的数据输出流队列，返回的是一个ByteBuffer数组
dequeueOutputBuffer：从输出队列中取出编码操作之后的数据
releaseOutputBuffer：处理完成，释放ByteBuffer数据
3、流量控制
对 TCP 来说就是控制单位时间内发送数据包的数据量，对编码来说就是控制单位时间内输出数据的数据量。
视频编码的限制条件最初是解码器的能力，码率太高就会无法解码，后来随着 codec 的发展，解码能力不再是瓶颈，限制条件变成了传输带宽/文件大小，我们希望在控制数据量的前提下，画面质量尽可能高。
码率：数字音频或视频编码中每秒传输的比特数
一般编码器都可以设置一个目标码率，但编码器的实际输出码率不会完全符合设置，因为在编码过程中实际可以控制的并不是最终输出的码率，而是编码过程中的一个量化参数（Quantization Parameter，QP），它和码率并没有固定的关系，而是取决于图像内容。

4、Android 硬编码流控
码率控制模式有三种：
CQ 表示完全不控制码率，尽最大可能保证图像质量；
CBR 表示编码器会尽量把输出码率控制为设定值，即我们前面提到的“不为所动”；
VBR 表示编码器会根据图像内容的复杂度（实际上是帧间变化量的大小）来动态调整输出码率，图像复杂则码率高，图像简单则码率低

配置时设置目标码率和码率控制模式
```java
mediaFormat.setInteger(MediaFormat.KEY_BIT_RATE, bitRate);
mediaFormat.setInteger(MediaFormat.KEY_BITRATE_MODE,
MediaCodecInfo.EncoderCapabilities.BITRATE_MODE_VBR);
mVideoCodec.configure(mediaFormat, null, null, MediaCodec.CONFIGURE_FLAG_ENCODE);
```
动态调整目标码率(Android 19 版本以上)
```java
Bundle param = new Bundle();
param.putInt(MediaCodec.PARAMETER_KEY_VIDEO_BITRATE, bitrate);
mediaCodec.setParameters(param);
```
## 录制流程
1、预览使用SurfaceView 视频采集使用Camera 音频使用AudioRecord
2、MediaCodec进行编码压缩 视频H264 音频AAC
3、Media Muxer 合成MP4
```java
```



# OpenGL ES
Android 能够通过framework框架提供的API或者NDK来支持OpenGL。
1、GLSurfaceView类 视图类
通过创建一个SurfaceView的实例并添加你的渲染器来使用这个类
2、GLSurfaceView.Renderer -- 绘制图形所需
GLSurfaceView.Renderer要求实现以下方法：
onSurfaceCreated()：创建GLSurfaceView时，系统调用一次该方法。使用此方法执行只需要执行一次的操作，例如设置OpenGL环境参数或初始化OpenGL图形对象。
onDrawFrame()：系统在每次重画GLSurfaceView时调用这个方法。使用此方法作为绘制（和重新绘制）图形对象的主要执行方法。
onSurfaceChanged()：当GLSurfaceView的发生变化时，系统调用此方法，这些变化包括GLSurfaceView的大小或设备屏幕方向的变化。例如：设备从纵向变为横向时，系统调用此方法。我们应该使用此方法来响应GLSurfaceView容器的改变。

