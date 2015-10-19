# obj-m is a list of what kernel modules to build
# KDIR is the location of the kernel source
# PWD is the current working directory and the location of our module 

obj-m := mousesimulator.o 
KDIR  := /lib/modules/$(shell uname -r)/build
PWD   := $(shell pwd)

default:
		$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
		rm -fr *.ko *.o
