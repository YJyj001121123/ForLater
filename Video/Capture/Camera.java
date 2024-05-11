
//https://blog.csdn.net/Sophia_hui/article/details/118388445
//https://yumore.github.io/2021/04/14/%E9%9F%B3%E8%A7%86%E9%A2%91%E7%9F%A5%E8%AF%86%E7%82%B9%E6%B1%87%E6%80%BB/
public class Camera {
    //Camera分为四层
    //应用层：直接调用Camera API,API会通过Binder和Framework层相机服务进行操作与数据的传递
    //Framework层：于 frameworks/av/services/camera/libcameraservice/CameraService.cpp ,相机Framework服务是承上启下的作用,上与应用交互,下与HAL曾交互
    AIDL HIDL
    //HAL层：硬件抽象层,Android 定义好了Framework服务与HAL层通信的协议及接口,HAL层如何实现有各个Vendor（供应商）自己实现,如Qcom的老架构mm-Camera,新架构Camx架构,Mtk的P之后的Hal3架构
    //Driver层：sensor 驱动层,数据由硬件到驱动层处理,驱动层接收HAL层数据以及传递Sensor数据给到HAL层,这里当然是各个Sensor芯片不同驱动也不同

    //MainActivity.java中使用SurfaceView或者SurfaceTexture+TextureView或者GLSurfaceView等控件作为显示预览界面的控件,共同点都是包含了一个单独的Surface作为取相机数据的容器
    //MainActivity.java onCreate的时候调用API 去通知Framework Native Service CameraServer去connect HAL继而打开Camera硬件sensor
    //openCamera成功后回调，CameraServer通知上层去initView。
    //同时，创建CameraCaptureSession,创建过程中会向CameraServer调用ConfigureStream的操作,ConfigureStream的参数中包含了第一步中空间中的Surface的引用,相当于App将Surface容器给到了CameraServer,CameraServer包装了下该Surface容器为stream,通过HIDL传递给HAL,继而HAL也做configureStream操作
    //ConfigureStream成功后CameraServer会给App回调通知ConfigStream成功,接下来App便会调用AOSP setRepeatingRequest接口给到CameraServer,CameraServer初始化时便起来了一个死循环线程等待来接收Request.
    //CameraServer将request交到Hal层去处理,得到HAL处理结果后取出该Request的处理Result中的Buffer填到App给到的容器中,SetRepeatingRequest为了预览,则交给Preview的Surface容器,如果是Capture Request则将收到的Buffer交给ImageReader的Surface容器
    //Surface本质上是BufferQueue的使用者和封装者,当CameraServer中App设置来的Surface容器被填满了BufferQueue机制将会通知到应用,此时App中控件取出各自容器中的内容消费掉,Preview控件中的Surface中的内容将通过View提供到SurfaceFlinger中进行合成最终显示出来,即预览;而ImageReader中的Surface被填了,则App将会取出保存成图片文件消费掉

    //APP层就是调用接口
    //openCamera：sensor 上电
    //configureStream：就是将控件如GLSurfaceView,ImageReader等中的Surface容器给到CameraServer
    //request：回馈到CameraServer
    //CameraServer将Request的处理结果Buffer数据填到对应的Surface容器中,填完后由BufferQueue机制回调到引用层对应的Surface控件的CallBack处理函数,接下来要显示预览或保图片App中对应的Surface中都有数据了

    //Framework层
    //就是CameraServer的实现
    //init启动有一个.rc文件去启动--->new SERVICE()
    //CameraService::instantiate()--->本质是BinderSevice的方法
    //再去构造CameraProviderManager
    //CameraProviderManager会去设置callback到HAL层
    //调用HAL接口，获得Provider的ProviderInfo

    //App调用Framework：
    //openCamera:
    //onCreate调用后，去OpenCamera
    //CameraService.connectDevice()(会走到HAL3层）--->return CameraDeviceUser
    //同时device封装层会给app 返回mDeviceCallback.onOpened()
    //收到回调后，去startPreview
    //createCaptureSession:
    //将Surface容器通过device层，传给CameraService-->HAL3
    //返回成功回调，等待request，通道建立完毕了
    //Request
    //现在开始，应用层发送指令，HAL开始向surface填充数据
    //Flush Close:
    //flush 处理掉正在处理掉request

    //HAL3层
    //应用向相机子系统发出request,一个request对应一组结果.request中包含所有配置信息。其中包括分辨率和像素格式；手动传感器、镜头和闪光灯控件；3A 操作模式；RAW 到 YUV 处理控件；以及统计信息的生成等.一次可发起多个请求，而且提交请求时不会出现阻塞。请求始终按照接收的顺序进行处理
    //request中携带了数据容器Surface,交到framework cameraserver中,打包成Camera3OutputStream实例,在一次CameraCaptureSession中包装成Hal request交给HAL层处理. Hal层获取到处理数据后返回給CameraServer,即CaptureResult通知到Framework,Framework cameraserver则得到HAL层传来的数据给他放进Stream中的容器Surface中.而这些Surface正是来自应用层封装了Surface的控件,这样App就得到了相机子系统传来的数据.

    //Camera2的新特性：
    //不开启相机检测相机信息。CameraCharacteristics实例
    //不开启预览进行拍照。
    //一次拍摄多张
    //灵活3A
}
//拍摄单张
//定义回调：
//onImageAvailable() 存储图像数据的image
//onCaptureCompleted() 存储拍照信息的 CaptureResult，它们是一一对应
private val captureResults: BlockingQueue<CaptureResult> = LinkedBlockingDeque()
private inner class CaptureImageStateCallback : CameraCaptureSession.CaptureCallback() {
    @MainThread
    override fun onCaptureCompleted(session: CameraCaptureSession, request: CaptureRequest, result: TotalCaptureResult) {
        super.onCaptureCompleted(session, request, result)
        captureResults.put(result)
    }
}

private inner class OnJpegImageAvailableListener : ImageReader.OnImageAvailableListener {
    @WorkerThread
    override fun onImageAvailable(imageReader: ImageReader) {
        val image = imageReader.acquireNextImage()
        val captureResult = captureResults.take()
        if (image != null && captureResult != null) {
            // Save image into sdcard.
        }
    }
}
//创建ImagerReader获取它的surface
val imageSize = getOptimalSize(cameraCharacteristics, ImageReader::class.java, maxWidth, maxHeight)!!
jpegImageReader = ImageReader.newInstance(imageSize.width, imageSize.height, ImageFormat.JPEG, 5)
jpegImageReader?.setOnImageAvailableListener(OnJpegImageAvailableListener(), cameraHandler)
jpegSurface = jpegImageReader?.surface
//创建CaptureRequest
captureImageRequestBuilder = cameraDevice.createCaptureRequest(CameraDevice.TEMPLATE_STILL_CAPTURE)
//TEMPLATE_STILL_CAPTURE：创建一个适合于静态图像捕获的请求，图像质量优先于帧速率。
//TEMPLATE_PREVIEW：创建预览的请求
//TEMPLATE_RECORD：创建视频录制的请求
// TEMPLATE_VIDEO_SNAPSHOT：创建视视频录制时截屏的请求
// TEMPLATE_ZERO_SHUTTER_LAG：创建一个适用于零快门延迟的请求。在不影响预览帧率的情况下最大化图像质量。
// TEMPLATE_MANUAL：创建一个基本捕获请求，这种请求中所有的自动控制都是禁用的(自动曝光，自动白平衡、自动焦点)。
captureImageRequestBuilder.addTarget(previewDataSurface)
captureImageRequestBuilder.addTarget(jpegSurface)



//Camera 2
//CameraCaptureSession-----> CaptureCallback\StateCallback
//APP预览拍照是都需要这个类创建session的
//控制预览方法setRepeatingRequese()
//控制拍照方法capture()

//CameraDevice、CameraCharacteristics---->StateCallback
//描述硬件设备以及设备的可用设置和输出参数。摄像头特性
//通过getCameraCharacteristics获得

//CameraManager
//相机设备的管理者

//CaptureRequest
//相机设备捕获单个映像所需的所有捕获参数
//列出了哪些配置的输出表面应该用作此捕获的目标


// 1.调用openCamera方法后会回调CameraDevice.StateCallback这个方法，在该方法里重写onOpened函数。
// 2.在onOpened方法中调用createCaptureSession，该方法又回调CameraCaptureSession.StateCallback方法。
// 3.CameraCaptureSession.StateCallback中重写onConfigured方法，设置setRepeatingRequest方法（也就是开启预览）。
// 4.setRepeatingRequest又会回调 CameraCaptureSession.CaptureCallback方法。
// 5.重写CameraCaptureSession.CaptureCallback中的onCaptureCompleted方法，result就是未经过处理的帧数据了。

CameraCharacteristics{
    LENS_FACING //前置摄像头（LENS_FACING_FRONT）还是后置摄像头（LENS_FACING_BACK）。
    SENSOR_ORIENTATION //拍照方向
    FLASH_INFO_AVAILABLE //是否有闪光灯
    INFO_SUPPORTED_HARDWARE_LEVEL //相机级别
}

//采用textureView
//onResume()的时候，进行判断，如果当前TextureView 可用了，则执行打开摄像头的操作，否则等待TextureView available
//第一次打开，TextureView还没创建完成，所以会在TextureView available回调中执行打开camera的操作
@Override
    public void onResume() {
        super.onResume();
            // 启动 HandlerThread ，后台维护一个 handler
        startBackgroundThread();

        if (mTextureView.isAvailable()) {
            openCamera(mTextureView.getWidth(), mTextureView.getHeight());
        } else {
            mTextureView.setSurfaceTextureListener(mSurfaceTextureListener);
        }
    }

private final TextureView.SurfaceTextureListener mSurfaceTextureListener = new TextureView.SurfaceTextureListener() {
@Override
public void onSurfaceTextureAvailable(SurfaceTexture texture, int width, int height) {
    openCamera( width, height);
}

@Override
public void onSurfaceTextureSizeChanged(SurfaceTexture texture, int width, int height) {
    configureTransform(width, height);
}

@Override
public boolean onSurfaceTextureDestroyed(SurfaceTexture texture) {
    return true;
}

@Override
public void onSurfaceTextureUpdated(SurfaceTexture texture) {
}
};
//openCamera实现
private void openCamera(int width, int height) {
    // 判断相机权限 6.0 以上的动态权限
    if (ContextCompat.checkSelfPermission(getActivity(), Manifest.permission.CAMERA)
            != PackageManager.PERMISSION_GRANTED) {
        requestCameraPermission();
        return;
    }
    setUpCameraOutputs(width, height);         // 配置 相机的 预览尺寸
    configureTransform(width, height);
    Activity activity = getActivity();
    CameraManager manager = (CameraManager) activity.getSystemService(Context.CAMERA_SERVICE);
    try {
        if (!mCameraOpenCloseLock.tryAcquire(2500, TimeUnit.MILLISECONDS)) {
            throw new RuntimeException("Time out waiting to lock back camera opening.");
        }
        manager.openCamera(mCameraId,mStateCallback, mBackgroundHandler);//打开状态在mStateCallback回调了
    } catch (CameraAccessException e) {
        e.printStackTrace();
    } catch (InterruptedException e) {
        throw new RuntimeException("Interrupted while trying to lock camera opening.", e);
    }
}
//mStateCallback
private final CameraDevice.StateCallback mStateCallback = new CameraDevice.StateCallback() {
    @Override
    public void onOpened(@NonNull CameraDevice cameraDevice) {
        // This method is called when the camera is opened.  We start camera preview here.
        mCameraOpenCloseLock.release();
        mCameraDevice = cameraDevice;
        //打开成功了 创建session 预览
        createCameraPreviewSession();
    }

    @Override
    public void onDisconnected(@NonNull CameraDevice cameraDevice) {
        mCameraOpenCloseLock.release();
        cameraDevice.close();
        mCameraDevice = null;
    }

    @Override
    public void onError(@NonNull CameraDevice cameraDevice, int error) {
        mCameraOpenCloseLock.release();
        cameraDevice.close();
        mCameraDevice = null;
        Activity activity = getActivity();
        if (null != activity) {
            activity.finish();
        }
    }
};
//createCameraPreviewSession
//图像显示在哪里？绑定surface
//发送预览的request
//建立session
private void createCameraPreviewSession() {
    try {
        SurfaceTexture texture = mTextureView.getSurfaceTexture();
        assert texture != null;
        // 我们将默认缓冲区的大小配置为我们想要的相机预览大小。
        // We configure the size of default buffer to be the size of camera preview we want.
        texture.setDefaultBufferSize(mPreviewSize.getWidth(), mPreviewSize.getHeight());
        //这是我们需要开始预览的输出surface
        // This is the output Surface we need to start preview.
        Surface surface = new Surface(texture);
        // 我们设置了一个CaptureRequest
        // We set up a CaptureRequest.Builder with the output Surface.
        mPreviewRequestBuilder = mCameraDevice.createCaptureRequest(CameraDevice.TEMPLATE_PREVIEW);
        mPreviewRequestBuilder.addTarget(surface);
        //request builder可以设置多个target，如果需要拿到实时的预览数据，则把imageReader 的surface 也设进去
    // mPreviewRequestBuilder.addTarget(mImageReader.getSurface());
        // 我们创建一个CameraCaptureSession用于相机预览。
        // Here, we create a CameraCaptureSession for camera preview.
        mCameraDevice.createCaptureSession(Arrays.asList(surface, mImageReader.getSurface()),
                new CameraCaptureSession.StateCallback() {
                    @Override
                    public void onConfigured(@NonNull CameraCaptureSession cameraCaptureSession) {        
                        // The camera is already closed
                        if (null == mCameraDevice) {
                            return;
                        }
                        // When the session is ready, we start displaying the preview.
                        mPreviewCaptureSession = cameraCaptureSession;
                        try {
                            // Auto focus should be continuous for camera preview.
                            mPreviewRequestBuilder.set(CaptureRequest.CONTROL_AF_MODE,
                                    CaptureRequest.CONTROL_AF_MODE_CONTINUOUS_PICTURE);
                            // Flash is automatically enabled when necessary.
                            setAutoFlash(mPreviewRequestBuilder);

                            // Finally, we start displaying the camera preview.
                            mPreviewRequest = mPreviewRequestBuilder.build();
//                                mPreviewCaptureSession.setRepeatingRequest(mPreviewRequest,
//                                        mCaptureCallback, mBackgroundHandler);
                            mPreviewCaptureSession.setRepeatingRequest(mPreviewRequest,
                                    null, mBackgroundHandler);
                        } catch (CameraAccessException e) {
                            e.printStackTrace();
                        }
                    }
                    @Override
                    public void onConfigureFailed(
                            @NonNull CameraCaptureSession cameraCaptureSession) {
                        showToast("Failed");
                    }
                }, null
        );
    } catch (CameraAccessException e) {
        e.printStackTrace();
    }
}
//capture()
// 1、首先需要初始化一个JPEG类型的ImageReader，用来接收底层数据回调；
private void initJpegImageReader(int width, int height) {
      
    StreamConfigurationMap map = mCameraCharacteristics.get(
              CameraCharacteristics.SCALER_STREAM_CONFIGURATION_MAP);
      if (map == null) {
          return;
      }
      
   Size largest = Collections.max(
               Arrays.asList(map.getOutputSizes(ImageFormat.JPEG)),
               new CompareSizesByArea());

     mJpegCpatureWidth = largest.getWidth();
     mJpegCaptureHeight = largest.getHeight();
     

   mJpegImageReader = ImageReader.newInstance(largest.getWidth(), largest.getHeight(),
               ImageFormat.JPEG, 2);
   mJpegImageReader.setOnImageAvailableListener(mJpegImageAvailableListener, mBackgroundHandler);
}
// 2、设置CameraDevice.TEMPLATE_STILL_CAPTURE 类型的请求，请求拍照；请求成功后，我们需要恢复正常的预览类型请求；
private void captureStillPicture() {
       
    try {
        if (null == mCameraDevice || mCapturing || mPreviewSession == null) {
    
            return;
        }
        
        mCapturing = true;

        // This is the CaptureRequest.Builder that we use to take a picture.
        mPreviewBuilder =
//设置拍照请求                    
mCameraDevice.createCaptureRequest(CameraDevice.TEMPLATE_STILL_CAPTURE);
        mPreviewBuilder.addTarget(mJpegImageReader.getSurface());

        // Use the same AE and AF modes as the preview.
//            mPreviewBuilder.set(CaptureRequest.CONTROL_AF_MODE,
//                    CaptureRequest.CONTROL_AF_MODE_CONTINUOUS_PICTURE);
//            setAutoFlash(captureBuilder);

        // Orientation
        int rotation = 0;//activity.getWindowManager().getDefaultDisplay().getRotation();
        mPreviewBuilder.set(CaptureRequest.JPEG_ORIENTATION, 0);

        CameraCaptureSession.CaptureCallback captureCallback
                = new CameraCaptureSession.CaptureCallback() {
            @Override
            public void onCaptureCompleted(@NonNull CameraCaptureSession session,
                                           @NonNull CaptureRequest request,
                                           @NonNull TotalCaptureResult result) {
            
                  //拍照请求成功后，恢复正常的预览模式
                startPreview();
              
              mCapturing = false;
            }
        };
        mPreviewSession.stopRepeating();
        mPreviewSession.abortCaptures();
        mPreviewSession.capture(mPreviewBuilder.build(),captureCallback , mBackgroundHandler);
    } catch (CameraAccessException e) {
        e.printStackTrace();
    }
}
// 3、在ImageReader回调中将接收到的jpeg数据进行保存；
private final ImageReader.OnImageAvailableListener mJpegImageAvailableListener = new ImageReader.OnImageAvailableListener() {
    @Override
    public void onImageAvailable(ImageReader reader) {
        Log.v(TAG, "--- mJpegImageAvailableListener();reader: " + reader);
        Image image = reader.acquireLatestImage();
        if(image == null){
          return;
        }
        ByteBuffer buffer = image.getPlanes()[0].getBuffer();
            byte[] bytes = new byte[buffer.remaining()];
            buffer.get(bytes);
        
            //将接收到的数据交由独立的线程进行文件的保存操作
        mBackgroundHandler.post(new ImageSaver(bytes,mJpegCpatureWidth,mJpegCaptureHeight, generateJpegFile(),mCaptureListener));
        image.close();
    }
};

//数码变焦
//CameraCharacteristics.SCALER_AVAILABLE_MAX_DIGITAL_ZOOM --最大倍数
//CaptureRequest.SCALER_CROP_REGION 裁剪范围

//对焦
//CONTROL_AF_MODE 


