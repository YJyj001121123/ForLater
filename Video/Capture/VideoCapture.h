#include "VideoInterface.h"

#include <windows.h>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <stdio.h>

#pragma comment(lib, "mfplat")
#pragma comment(lib, "mfuuid")
#pragma comment(lib, "mfreadwrite")

#ifndef FORLATER_VIDEOCAPTURE_H
#define FORLATER_VIDEOCAPTURE_H

#endif //FORLATER_VIDEOCAPTURE_H
class VideoCaptureInterface{};

/***
 * 视频功能实现的各个平台层实现
*/
class VideoAndroidCapture:public VideoCaptureInterface{
    /***
     * C++调用JAVA jni
     */
};

class VideoAppleCapture:public VideoCaptureInterface{
    /***
     * OC实现 混编
     * AVCaptureSession
     * 
     * 创建AVCaptureSession实例
     * 获取AVCaptureDevice 添加到AVCaptureSession
     * 创建AVCaptureDeviceInput  添加到AVCaptureSession
     * 创建AVCaptureOutput 
     */
};

class ViewController: UIViewController, AVCaptureVideoDataOutputSampleBufferDelegate {

    var captureSession: AVCaptureSession!
    var videoPreviewLayer: AVCaptureVideoPreviewLayer!
    
    override func viewDidLoad() {
        super.viewDidLoad()
        
        // 创建 AVCaptureSession 实例
        captureSession = AVCaptureSession()
        
        // 获取摄像头设备
        guard let captureDevice = AVCaptureDevice.default(for: .video) else { return }
        
        // 创建 AVCaptureDeviceInput 实例
        do {
            let input = try AVCaptureDeviceInput(device: captureDevice)
            captureSession.addInput(input)
        } catch {
            print(error.localizedDescription)
            return
        }
        
        // 创建 AVCaptureVideoDataOutput 实例
        let videoOutput = AVCaptureVideoDataOutput()
        videoOutput.setSampleBufferDelegate(self, queue: DispatchQueue(label: "videoQueue"))
        captureSession.addOutput(videoOutput)
        
        // 创建预览图层
        videoPreviewLayer = AVCaptureVideoPreviewLayer(session: captureSession)
        videoPreviewLayer.videoGravity = .resizeAspectFill
        videoPreviewLayer.frame = view.bounds
        view.layer.addSublayer(videoPreviewLayer)
        
        // 开始采集
        captureSession.startRunning()
    }
    
    // AVCaptureVideoDataOutputSampleBufferDelegate 方法，处理采集的视频帧数据
    func captureOutput(_ output: AVCaptureOutput, didOutput sampleBuffer: CMSampleBuffer, from connection: AVCaptureConnection) {
        // 在这里处理视频帧数据，例如实时显示、保存等操作
        print("Captured a frame")
    }
}


class VideoLinuxCapture:public VideoCaptureInterface{
    /***
     * C++
     * V4L2 USB摄像头获取视频帧
     * 
     * 打开设备：
     * 首先，应用程序需要打开要使用的视频设备。这可以通过使用 open() 系统调用来完成，
     * 通常是打开 /dev/video0 或类似的设备文件。
     * 
     * 设置设备参数：
     * 在打开设备后，需要设置设备的各种参数，
     * 例如视频格式、分辨率、帧率等。这可以通过使用 ioctl() 系统调用来完成，
     * 指定不同的控制命令（VIDIOC_S_FMT 等）来设置参数
     * 
     * 请求缓冲区：一旦设置了设备参数，应用程序需要为视频帧请求缓冲区。
     * 这可以通过使用 ioctl() 系统调用和 VIDIOC_REQBUFS 控制命令来完成。
     * 设备将为应用程序提供一些空闲的缓冲区，用于存储视频帧数据。
     * 
     * 映射缓冲区：应用程序需要将这些缓冲区映射到应用程序的内存空间中，以便可以直接访问它们。
     * 这通常通过使用 mmap() 系统调用来完成。
     * 
     * 启动采集：一旦缓冲区准备就绪并映射到应用程序的地址空间中，应用程序可以开始启动视频采集。
     * 这可以通过使用 ioctl() 系统调用和 VIDIOC_STREAMON 控制命令来完成。
     * 
     * 采集数据：设备开始将视频数据写入应用程序提供的缓冲区中。
     * 应用程序可以定期检查这些缓冲区以获取新的视频帧。
     * 
     * 处理数据：应用程序可以对采集到的视频帧进行任何必要的处理，例如图像处理、编码、分析等。
     * 
     * 停止采集：当不再需要采集时，应用程序可以通过使用 ioctl() 系统调用和 VIDIOC_STREAMOFF 控制命令来停止视频采集。
     * 
     * 关闭设备：最后，应用程序需要关闭视频设备，释放资源。这可以通过使用 close() 系统调用来完成。
     */

}

class VideoWindowsCapture:public VideoCaptureInterface{
    /***
     * C++ 采用MF media Foundation
     * IMFSinkWriter *pWriter = NULL;//可以将采集到的音视频数据写入到文件中，同时自动处理格式转换、编码和封装等操作
     * 
     * //创建设备拓扑：表示视频采集所涉及的设备和数据流 MFCreateMediaSession
     * //选择视频源：确定要从哪个视频源采集视频。
     * //配置视频格式：设置视频采集的格式、分辨率、帧率等参数。 IMFMediaType
     * //创建采集会话
     * //设置回调函数：为视频采集会话设置回调函数，以便在每次采集到新的视频帧时进行处理IMFCaptureEngineOnSampleCallback
     * //启动采集：StartPreview() 或 StartRecord()
     * //采集数据
     * //停止采集：StopPreview() 或 StopRecord()
     */
};

int main() {
    HRESULT hr = S_OK;
    IMFMediaSource *pSource = NULL; //媒体源
    IMFAttributes *pAttributes = NULL; //属性
    IMFMediaType *pType = NULL;
    IMFSinkWriter *pWriter = NULL;

    // 初始化 Media Foundation
    hr = MFStartup(MF_VERSION);
    if (FAILED(hr)) {
        printf("Error initializing Media Foundation: %#X\n", hr);
        return hr;
    }

    // 创建源读取器
    hr = MFCreateAttributes(&pAttributes, 1);
    if (FAILED(hr)) {
        printf("Failed to create attributes: %#X\n", hr);
        goto cleanup;
    }

    hr = pAttributes->SetGUID(MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE, MF_DEVSOURCE_ATTRIBUTE_SOURCE_TYPE_VIDCAP_GUID);
    if (FAILED(hr)) {
        printf("Failed to set device attribute: %#X\n", hr);
        goto cleanup;
    }

    hr = MFCreateDeviceSource(pAttributes, &pSource);
    if (FAILED(hr)) {
        printf("Failed to create device source: %#X\n", hr);
        goto cleanup;
    }

    // 创建视频输出文件
    hr = MFCreateSinkWriterFromURL(L"output.mp4", NULL, NULL, &pWriter);
    if (FAILED(hr)) {
        printf("Failed to create sink writer: %#X\n", hr);
        goto cleanup;
    }

    // 设置输出格式
    hr = MFCreateMediaType(&pType);
    if (FAILED(hr)) {
        printf("Failed to create media type: %#X\n", hr);
        goto cleanup;
    }

    hr = pType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
    if (FAILED(hr)) {
        printf("Failed to set major type: %#X\n", hr);
        goto cleanup;
    }

    hr = pType->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_H264);
    if (FAILED(hr)) {
        printf("Failed to set subtype: %#X\n", hr);
        goto cleanup;
    }

    hr = pWriter->AddStream(pType, NULL);
    if (FAILED(hr)) {
        printf("Failed to add stream: %#X\n", hr);
        goto cleanup;
    }

    hr = pWriter->SetInputMediaType(0, pType, NULL);
    if (FAILED(hr)) {
        printf("Failed to set input media type: %#X\n", hr);
        goto cleanup;
    }

    // 开始写入
    hr = pWriter->BeginWriting();
    if (FAILED(hr)) {
        printf("Failed to begin writing: %#X\n", hr);
        goto cleanup;
    }

    printf("Recording started. Press Enter to stop...\n");
    getchar();

cleanup:
    if (pSource) pSource->Release();
    if (pAttributes) pAttributes->Release();
    if (pType) pType->Release();
    if (pWriter) {
        pWriter->Finalize();
        pWriter->Release();
    }
    MFShutdown();
    return 0;
}


class VECameraWrapper:public VideoCaptureInterface{
    /***
     * Android/iOS VEcamera 摄像头功能
     */
};

/***
 * 实现外部接口的外部源视频数据接收的实现类
 * 在逻辑上模拟与摄像头采集实现一致对外的视频帧的来源和视频帧推送接口。
 * 当开启视频外部源时，实现对摄像头采集的平台实现的直接替换即开启外部采集
 * 并接收外部视频帧，内部在pipeline中传递处理
*/
class VideoExternalCapture : public VideoCaptureInterface{
/***
 * 外部源
*/
};
