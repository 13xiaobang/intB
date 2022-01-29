#include <binder/BinderService.h>
#include <binder/IAppOpsCallback.h>
#include <binder/IPCThreadState.h>
#include <binder/Binder.h>
#include <binder/BpBinder.h>
#include <gui/SurfaceComposerClient.h>
#include <gui/BufferQueueCore.h>
#include <gui/ISurfaceComposer.h>
#include <gui/Surface.h>
#include <gui/SurfaceComposerClient.h>
#include <gui/SurfaceControl.h>
#include <android/native_window.h>
#include <SkBitmap.h>
#include <SkCanvas.h>
#include <SkPaint.h>
#include <SkSurface.h>
#include <SkData.h>
#include <GLES/gl.h>
#include <GLES/glext.h>
#include <EGL/eglext.h>
using namespace android;
//先写一个函数把图片转成一个bitmap

static inline SkColorType convertPixelFormat(PixelFormat format) {
    /* note: if PIXEL_FORMAT_RGBX_8888 means that all alpha bytes are 0xFF, then
        we can map to kN32_SkColorType, and optionally call
        bitmap.setAlphaType(kOpaque_SkAlphaType) on the resulting SkBitmap
        (as an accelerator)
    */
    switch (format) {
    case PIXEL_FORMAT_RGBX_8888:    return kN32_SkColorType;
    case PIXEL_FORMAT_RGBA_8888:    return kN32_SkColorType;
    case PIXEL_FORMAT_RGB_565:      return kRGB_565_SkColorType;
    default:                        return kUnknown_SkColorType;
    }
}

static status_t initBitmap(SkBitmap* bitmap, const char* fileName) {
    if (fileName == NULL) {
        return NO_INIT;
    }
    SkAutoDataUnref data(SkData::NewFromFileName(fileName));
    SkImage* image = SkImage::NewFromEncoded(data);
    //sk_sp<SkImage> image = SkImage::MakeFromEncoded(data);
     bool  result  = image->asLegacyBitmap(bitmap, SkImage::kRO_LegacyBitmapMode);
    if(!result ){
        printf("decode picture fail!");
        return NO_INIT;
    }
    return NO_ERROR;
}

int main()
{
    sp<ProcessState> proc(ProcessState::self());
    ProcessState::self()->startThreadPool();//和上一示例一样要开启binder线程池

    // create a client to surfaceflinger
    sp<SurfaceComposerClient> client = new SurfaceComposerClient();//三大件第一件
    client->initCheck();
    sp<SurfaceControl> surfaceControl = client->createSurface(String8("Consoleplayer Surface"),1280, 720, PIXEL_FORMAT_RGBA_8888);//三大件第二件

    //SurfaceComposerClient::Transaction t;
    //t.setLayer(surfaceControl, 0x40000000).apply();
    SurfaceComposerClient::openGlobalTransaction();
    surfaceControl->setLayer(220000);
    SurfaceComposerClient::closeGlobalTransaction();
    sp<Surface> surface = surfaceControl->getSurface();//三大件第三件
    
    // initialize opengl and egl
    const EGLint attribs[] = {
            EGL_RED_SIZE,   8,
            EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE,  8,
            EGL_DEPTH_SIZE, 0,
            EGL_NONE
    };
    
    //开始初始化EGL库
    EGLint w, h;
    EGLSurface eglSurface;
    EGLint numConfigs;
    EGLConfig config;
    EGLContext context;
    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    
    eglInitialize(display, 0, 0);
    eglChooseConfig(display, attribs, &config, 1, &numConfigs);
    eglSurface = eglCreateWindowSurface(display, config, surface.get(), NULL);//创建eglSurface(对Surface的一个封装)
    context = eglCreateContext(display, config, NULL, NULL);
    eglQuerySurface(display, eglSurface, EGL_WIDTH, &w);
    eglQuerySurface(display, eglSurface, EGL_HEIGHT, &h);

    if (eglMakeCurrent(display, eglSurface, eglSurface, context) == EGL_FALSE)//会调用dequeue以获取帧缓冲区buffer
        return NO_INIT;

    glShadeModel(GL_FLAT);
    glDisable(GL_DITHER);
    glDisable(GL_SCISSOR_TEST);
    //draw red 
    glClearColor(255,0,0,1);//这里用opengl库来一个纯红色的画面
    glClear(GL_COLOR_BUFFER_BIT);
    
    eglSwapBuffers(display, eglSurface);//这里会调用到Surface的queueBuffer方法，提交画好的帧缓冲区数据


    printf("sleep...\n");
    usleep(25 * 1000 * 1000);
    
    surface.clear();
    surfaceControl.clear();
    
    printf("test complete. CTRL+C to finish.\n");
    IPCThreadState::self()->joinThreadPool();
    return 0;
}
