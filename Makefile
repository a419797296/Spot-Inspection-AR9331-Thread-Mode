.PHONY:clean
OBJECTS= Scoket Server.o socket_driver.o

spot_inspection_app:$(OBJECTS)
	mips-openwrt-linux-gcc -Wall -g $^ -o $@ -lpthread
Scoket Server.o:Scoket Server.c
	mips-openwrt-linux-gcc -Wall -g -c $< -o $@
socket_driver.o:socket_driver.c
	mips-openwrt-linux-gcc -Wall -g -c $< -o $@

clean:
	rm -f $(OBJECTS) spot_inspection_app
