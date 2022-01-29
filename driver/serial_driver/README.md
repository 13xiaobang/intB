# how to make misc_driver
1. set cross compile tool env.
2. modify "Makefile" to set CROSS_TOOLS & KERNEL_OUT_PATH.
3. run "make"
4. get misc_driver.ko

# how to make misc_user
1. install android-ndk-r23b
2. run "$(NDK_PATH)/ndk-build NDK_PROJECT_PATH=. NDK_APPLICATION_MK=Application.mk APP_BUILD_SCRIPT=Android.mk"
3. in libs/armeabi-v7a/, get misc_user

# how to run example on board
1. adb push misc_driver /data/
2. adb push misc_user /data/
3. insmod /data/misc_driver
4. chmod 777  /data/misc_user
5. /data/misc_user 
