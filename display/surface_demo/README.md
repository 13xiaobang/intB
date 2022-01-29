#Surface Demo

### surface_raw:
use raw surface to render.

### surface_opengl:
use opengl to render surface.


##
#How to build and run

1. copy the whole file dir to $(ANDROID_ROM_DIR)/frameworks/native/services/
2. . build/envsetup.sh && lunch to pick a combo 
3. cd to $(ANDROID_ROM_DIR)/frameworks/native/services/surface_demo
3. run "mm" to get surface_demo binary
4. adb push to board
5. run ./surface_demo on board. 