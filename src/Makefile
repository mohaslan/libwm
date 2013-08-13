all:
	cc -c libwm.c -o libwm.o -fpic
	cc -shared -Wl,-soname,libwm.so.1 -o libwm.so.1.0 libwm.o -lX11
	ln -sf libwm.so.1.0 libwm.so.1
	ln -sf libwm.so.1.0 libwm.so
clean:
	rm -f *.o *.so *.so.*

