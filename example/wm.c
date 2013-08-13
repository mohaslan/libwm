#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "libwm.h"

struct wm_context wm;


void my_keypress(char *keycode) {
	//printf("Key Pressed: %s\n", keycode);
}

void my_keyrelease(char *keycode) {
	int i, count;
	struct wm_window *windows;
	printf("Key Released: %s\n", keycode);
	if(strcmp(keycode, "t") == 0) {
		if(!fork())
			//execve("/usr/bin/xterm", NULL, NULL);
			system("/usr/bin/xterm");
	}
	else if(strcmp(keycode, "l") == 0) {
		count = wm_get_windows(&wm, &windows);
		printf("\nwindow list (%d):\n", count);
		for(i=0 ; i<count ; i++) {
			printf("win%d: %s - %s\n", i, windows[i].title, windows[i].icon);
			wm_move_window(&wm, &(windows[i]), 50, 70);
			wm_resize_window(&wm, &(windows[i]), 600, 600);
			//wm_set_border_width(&wm, &(windows[i]), 10);
			//wm_set_border_color(&wm, &(windows[i]), 255, 0, 0);
			wm_reparent(&wm, &(windows[i]), 10, 30);
		}
		printf("\n");
	}
	else if(strcmp(keycode, "x") == 0)
		exit(0);
}


int main() {
	wm_new(&wm, "MyWM");
	if(!wm_init(&wm)) {
		printf("Error.");
		return 0;
	}
	unsigned int width, height;
	wm_get_screen_resolution(&wm, &width, &height);
	printf("Screen: %dx%d\n", width, height);
	wm_set_background_color(&wm, 0, 255, 255);
	
	wm_set_cursor(&wm, CURSOR_DEFAULT);


	wm.key_press = my_keypress;
	wm.key_release = my_keyrelease;

	wm_event_loop(&wm);

	wm_close(&wm);
	return 0;
}

