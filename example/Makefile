CFLAGS += -I../src
LIBWM = ../src

all:
	cc -c wm.c -o wm.o $(CFLAGS)
	cc -Wl,-rpath=$(shell pwd)/$(LIBWM) -Wall -L$(LIBWM) wm.o -o wm -lX11 -lwm
clean:
	rm -f *.o wm

