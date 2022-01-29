CROSS_TOOL=arm-linux-androideabi-#set cross tool
KERNEL_OUT_PATH=/home/frank/g5sp/out_full_bx8691p1_1g/target/product/bx8691p1_1g# set kernel out path

PWD := $(shell pwd)
obj-m = misc_driver.o
KDIR := $(KERNEL_OUT_PATH)/obj/KERNEL_OBJ/
ccflags-y=-Wno-unused-result
all:
	$(MAKE) -C $(KDIR) M=$(PWD) ARCH=arm CROSS_COMPILE=$(CROSS_TOOL) modules

#install:all
#		$(MAKE) -C $(KDIR) M=$(PWD) modules_install

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
