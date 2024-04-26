package com.example.cameratest;

//https://juejin.cn/post/7062357163829624845
//https://github.com/android/camera-samples?tab=readme-ov-file
//https://source.android.google.cn/devices/camera/versioning
/***
 * Camera 客户端：Camera1 Camera2  CameraX
 * Camera HAL：
 * IPC: Binder HwBinder VndBinder
 * Camera Provider
 * HAL: Camx Chi
 * Camera Framework
 * CameraService:VINTF
 * Framework
 * 图像：
 * 录像：
 * OpenGL：
 */

/***
 * Camera整体框架主要包括三个进程：（进程间的通信通过binder实现的）
 * app进程和camera server进程间使用
 * AIDL(Android Interface Definition Language)通信
 * camera server进程和hal进程（provider进程）间使用
 * HIDL(HAL interface definition language) 通信
 * HIDL的目标是框架可以在无需重新构建HAL的情况下
 */
//Android 框架 ：应用层->framework层->Hal层（硬件抽象层）
//camera server复制app和framework层的通信，provider进程负责framework和hal层通信
//为了减少framework和hal的耦合，提出了HIDL，指定HAL和其用户之间的接口的一种接口描述语言
public class camera{
}
