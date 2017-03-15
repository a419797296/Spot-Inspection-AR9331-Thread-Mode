.PHONY:clean
<<<<<<< HEAD
OBJECTS= main.o socket_server.o socket_client.o socket_ser2net.o wifi_config.o socket_driver.o 
=======
OBJECTS= socket_server.o socket_driver.o
>>>>>>> 9b031e349cebb7b0149b220d87c0f635253c765f

spot_inspection_app:$(OBJECTS)
	mips-openwrt-linux-gcc -Wall  $^ -o $@ -lpthread -Wl,--rpath-link /opt/OpenWrt-Toolchain-ar71xx-for-mips_34kc-gcc-4.8-linaro_uClibc-0.9.33.2/toolchain-mips_34kc_gcc-4.8-linaro_uClibc-0.9.33.2/lib
socket_server.o:socket_server.c
	mips-openwrt-linux-gcc -Wall  -c $< -o $@ -lpthread
<<<<<<< HEAD
socket_client.o:socket_client.c
	mips-openwrt-linux-gcc -Wall  -c $< -o $@ -lpthread
socket_ser2net.o:socket_ser2net.c
	mips-openwrt-linux-gcc -Wall  -c $< -o $@ -lpthread
socket_driver.o:socket_driver.c
	mips-openwrt-linux-gcc -Wall  -c $< -o $@
main.o:main.c
	mips-openwrt-linux-gcc -Wall  -c $< -o $@
wifi_config.o:wifi_config.c
	mips-openwrt-linux-gcc -Wall  -c $< -o $@
=======
socket_driver.o:socket_driver.c
	mips-openwrt-linux-gcc -Wall  -c $< -o $@
>>>>>>> 9b031e349cebb7b0149b220d87c0f635253c765f

clean:
	rm -f $(OBJECTS) spot_inspection_app

