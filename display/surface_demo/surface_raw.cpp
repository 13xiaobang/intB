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
using namespace android;
int main()
{
    sp<ProcessState> proc(ProcessState::self());
    ProcessState::self()->startThreadPool();//在应用和SurfaceFlinger沟通过程中要使用到binder, 所以这里要先初始化binder线程池
    sp<SurfaceComposerClient> client = new SurfaceComposerClient();//SurfaceComposerClient是SurfaceFlinger在应用侧的代表， SurfaceFlinger的接口通过它来提供
    client->initCheck();
    //先通过createSurface接口来申请一块画布，参数里包含对画布起的名字，大小，位深信息
    sp<SurfaceControl> surfaceControl = client->createSurface(String8("Console Surface"),1280, 720, PIXEL_FORMAT_RGBA_8888);

    //SurfaceComposerClient::Transaction t;
    //t.setLayer(surfaceControl, 0x40000000).apply();
    SurfaceComposerClient::openGlobalTransaction();
    surfaceControl->setLayer(22000);
    SurfaceComposerClient::closeGlobalTransaction();
    //通过getSurface接口获取到Surface对象
    sp<Surface> surface = surfaceControl->getSurface();

    ANativeWindow_Buffer buffer;
    //通过Surface的lock方法调用到dequeueBuffer，获取到一个BufferQueue可用的Slot
    status_t err = surface->lock(&buffer, NULL);// &clipRegin

    void* addr = buffer.bits;
    ssize_t len = buffer.stride * 4 * buffer.height;
    memset(addr, 200, len);//这里绘图，由于我们没有使用任何图形库，所以这里把内存填成200， 画一个纯色画面
    printf("len=%d\n", len);
    surface->unlockAndPost();//这里会调用到queueBuffer,把我们绘制好的画面提交给SurfaceFlinger

    printf("sleep...\n");
    usleep(25 * 1000 * 1000);

    surface.clear();
    surfaceControl.clear();

    printf("complete. CTRL+C to finish.\n");
    IPCThreadState::self()->joinThreadPool();
    return 0;
}
