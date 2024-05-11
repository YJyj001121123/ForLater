public class MediaCodec {
    /***
     * MediaCodec类Android提供的用于访问低层多媒体编/解码器接口
     * MediaExtractor、MediaMuxer、AudioTrack结合使用，能够编解码诸如H.264、H.265、AAC、3gp等常见的音视频格式
     */
}
//创建编解码器
createEncoderByType（string type）
createDecoderByType(String type)
createByCodecName (String name) //使用组件的具体名称
//配置、启动
configure()
start()--调用低层方法ByteBuffer[] getBuffers(input)来开辟一系列输入、输出缓存区
MediaFormat mFormat = MediaFormat.createVideoFormat("video/avc",640,480);
mFormat.setInteger(MediaFormat.KEY_BIT_RATE,600); //比特率
mFormat.setInteger(MediaFormat.KEY_FRAME_RATE,30);  //帧率
mFormat.setInteger(MediaFormat.KEY_COLOR_FORMAT,mcf);  //颜色格式
//选择哪种颜色格式与输入的视频数据源颜色格式有关
//Camera预览采集的图像流通常为NV21或YV12，那么编码器需要指定相应的颜色格式，否则编码得到的数据可能会出现花屏、叠影、颜色失真等现象
mFormat.setInteger(MediaFormat.KEY_I_FRAME_INTERVAL,10);  //关键帧间隔
mVideoEncoder.configure(mFormat,MediaCodec.CONFIGURE_FLAG_ENCODE);
//数据处理
//同步syb-编解码器数据的输入和输出是同步的，编解码器只有处理输出完毕才会再次接收输入数据
//异步asyn-不会等待输出数据处理完毕才再次接收输入数据
// getInputBuffers：获取需要编码数据的输入流队列，返回的是一个ByteBuffer数组 ，已弃用
// queueInputBuffer：输入流入队列
// dequeueInputBuffer：从输入流队列中取数据进行编码操作
// getOutputBuffers：获取编解码之后的数据输出流队列，返回的是一个ByteBuffer数组 ，已弃用
// dequeueOutputBuffer：从输出队列中取出编码操作之后的数据
// releaseOutputBuffer：处理完成，释放ByteBuffer数据

//硬编
import android.media.MediaCodec;
import android.media.MediaCodecInfo;
import android.media.MediaFormat;
import android.os.Build;
import android.view.Surface;

import java.io.IOException;
import java.nio.ByteBuffer;

public class H264HardEncoder {
    private MediaCodec mediaCodec;

    public H264HardEncoder(int width, int height, int bitRate, int frameRate, Surface surface) throws IOException {
        MediaFormat format = MediaFormat.createVideoFormat(MediaFormat.MIMETYPE_VIDEO_AVC, width, height);
        format.setInteger(MediaFormat.KEY_BIT_RATE, bitRate);
        format.setInteger(MediaFormat.KEY_FRAME_RATE, frameRate);
        format.setInteger(MediaFormat.KEY_COLOR_FORMAT, MediaCodecInfo.CodecCapabilities.COLOR_FormatSurface);
        format.setInteger(MediaFormat.KEY_I_FRAME_INTERVAL, 1);

        mediaCodec = MediaCodec.createEncoderByType(MediaFormat.MIMETYPE_VIDEO_AVC);
        mediaCodec.configure(format, null, null, MediaCodec.CONFIGURE_FLAG_ENCODE);
        mediaCodec.setInputSurface(surface);
        mediaCodec.start();
    }

    public void encodeFrame(ByteBuffer buffer, long presentationTimeUs) {
        int inputBufferIndex = mediaCodec.dequeueInputBuffer(-1);
        if (inputBufferIndex >= 0) {
            ByteBuffer inputBuffer;
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
                inputBuffer = mediaCodec.getInputBuffer(inputBufferIndex);
            } else {
                inputBuffer = mediaCodec.getInputBuffers()[inputBufferIndex];
            }
            inputBuffer.clear();
            inputBuffer.put(buffer);
            mediaCodec.queueInputBuffer(inputBufferIndex, 0, buffer.limit(), presentationTimeUs, 0);
        }
    }

    public void release() {
        mediaCodec.stop();
        mediaCodec.release();
    }
}
//软编码
import android.media.MediaCodec;
import android.media.MediaCodecInfo;
import android.media.MediaFormat;

import java.io.IOException;
import java.nio.ByteBuffer;

public class H264SoftEncoder {
    private MediaCodec mediaCodec;

    public H264SoftEncoder(int width, int height, int bitRate, int frameRate) throws IOException {
        MediaFormat format = MediaFormat.createVideoFormat(MediaFormat.MIMETYPE_VIDEO_AVC, width, height);
        format.setInteger(MediaFormat.KEY_BIT_RATE, bitRate);
        format.setInteger(MediaFormat.KEY_FRAME_RATE, frameRate);
        format.setInteger(MediaFormat.KEY_COLOR_FORMAT, MediaCodecInfo.CodecCapabilities.COLOR_FormatYUV420Planar);
        format.setInteger(MediaFormat.KEY_I_FRAME_INTERVAL, 1);

        mediaCodec = MediaCodec.createEncoderByType(MediaFormat.MIMETYPE_VIDEO_AVC);
        mediaCodec.configure(format, null, null, MediaCodec.CONFIGURE_FLAG_ENCODE);
        mediaCodec.start();
    }

    public ByteBuffer[] encodeFrame(byte[] inputData) {
        int inputBufferIndex = mediaCodec.dequeueInputBuffer(-1);
        if (inputBufferIndex >= 0) {
            ByteBuffer inputBuffer = mediaCodec.getInputBuffers()[inputBufferIndex];
            inputBuffer.clear();
            inputBuffer.put(inputData);
            mediaCodec.queueInputBuffer(inputBufferIndex, 0, inputData.length, System.nanoTime() / 1000, 0);
        }

        MediaCodec.BufferInfo bufferInfo = new MediaCodec.BufferInfo();
        int outputBufferIndex = mediaCodec.dequeueOutputBuffer(bufferInfo, 0);
        if (outputBufferIndex >= 0) {
            ByteBuffer outputBuffer = mediaCodec.getOutputBuffers()[outputBufferIndex];
            return new ByteBuffer[]{outputBuffer};
        } else {
            return null;
        }
    }

    public void release() {
        mediaCodec.stop();
        mediaCodec.release();
    }
}




//FFMpeg https://juejin.cn/post/6844903961644793869
//基础：https://juejin.cn/post/6844904045572784142
//OPENGLES https://www.jianshu.com/p/762a406e9266