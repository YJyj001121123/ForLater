//初始旋转角设定： 重力旋转角 UI旋转角 自定义角度
//本端内部渲染旋转设置、编码旋转、自定义渲染旋转、单流转推（自定义视频前处理及编码前，设置视频帧的朝向）
//远端内部渲染旋转设置、自定义渲染旋转

//自适应功能：本远端自适应渲染，无横竖屏要求
//横（竖）屏帧：本端内部渲染、自定义渲染横（竖）屏  本端编码横（竖）屏  旋转方式-横（竖）屏帧

//Landscape模式
//图像正向，宽大于高
//Portrait模式
//图像正向，宽小于高
//Landscape模式 -- Portrait模式 转换
//缩放裁剪 、旋转（无法保证视频正向）

//- 推流端和拉流端 获得的视频帧分辨率 、 
//显示设备的分辨率存在不匹配（比如电脑、手机）
//为了解决不同的横竖屏更好显示Landscape 、Portrait的视频帧类型


//初始旋转角设定：
VideoCaptureInterface 满足
//Frame模式
在Effect Node实现