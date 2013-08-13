/*-
 * Copyright (c) 2013, by Mohamed Aslan
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. The name of the developer may NOT be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

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

