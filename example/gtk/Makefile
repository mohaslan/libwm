LIBWM = ../../src
CFLAGS += -I$(LIBWM)
GTK2 += $(shell pkg-config --cflags --libs gtk+-2.0)

all:
	cc -c gtkwm.c -o gtkwm.o $(CFLAGS) $(GTK2)
	cc -Wl,-rpath=$(shell pwd)/$(LIBWM) -Wall -L$(LIBWM) gtkwm.o -o gtkwm -lwm $(GTK2) 
clean:
	rm -f *.o gtkwm

