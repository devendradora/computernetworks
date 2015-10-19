Compile (need to install Kernel header):
$ make

Load:
$ sudo insmod softwaremouse.ko

Test:
# To switch to root
$ sudo su -
# Mouse X axis 50, Y-axis movement 0,0 representative pure movement does not click , Last 0 can be changed to 1 or 2 for clicking
$ echo "50 0 0" > /sys/devices/platform/mousesimulator/mouse_event

