LIBWM = ../../src
CFLAGS += -I$(LIBWM)

all:
	cc -c x11wm.c -o x11wm.o $(CFLAGS)
	cc -Wl,-rpath=$(shell pwd)/$(LIBWM) -Wall -L$(LIBWM) x11wm.o -o x11wm -lX11 -lwm -lXpm
clean:
	rm -f *.o x11wm

