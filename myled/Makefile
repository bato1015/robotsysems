obj-m :=2myled.o
myled.ko: 2myled.c
	make -C /usr/src/linux-headers-`uname -r` M=`pwd` V=1 modules
	gcc 2numeron.c -o numeron
clean:
	make -C /usr/src/linux-headers-`uname -r` M=`pwd` V=1 clean
	rm numeron
