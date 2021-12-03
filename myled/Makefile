obj-m :=myled.o
CC := gcc
myled.ko: myled.c
	make -C /usr/src/linux-headers-`uname -r` M=`pwd` V=1 modules
clean:
	make -C /usr/src/linux-headers-`uname -r` M=`pwd` V=1 clean
