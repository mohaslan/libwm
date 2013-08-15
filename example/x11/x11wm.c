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
#include <string.h>
#include "libwm.h"
#include <X11/xpm.h>
#include "exit_icon.xpm"

struct wm_context wm;
unsigned int screen_width, screen_height;

struct wm_window* create_parent_window(struct wm_window *child) {
	GC gc;
	XGCValues values;
	struct wm_window *parent_win = (struct wm_window *)malloc(sizeof(struct wm_window));
	XColor bg_color;
	bg_color.red = 0 * 256;
	bg_color.green = 100 * 256;
	bg_color.blue = 150 * 256;
	XAllocColor(wm.display, DefaultColormap(wm.display, wm.screen), &bg_color);
	parent_win->win = XCreateSimpleWindow(wm.display, DefaultRootWindow(wm.display), 0, 0, 600, 600, 0, bg_color.pixel, bg_color.pixel);
	wm_window_resize(&wm, child, 590, 570);

	int w, h;
	w = 100 + rand()%screen_width - (child->width + 20);
	h = 100 + rand()%screen_height - (child->height + 20);
	w = w >= 0 ? w : 0;
	h = h >= 0 ? h : 0;
	printf("::: %d %d\n", w, h);
	wm_window_move(&wm, parent_win, w, h);
	wm_window_show(&wm, parent_win);

   	gc = XCreateGC(wm.display, parent_win->win, None, &values);
	XSetLineAttributes(wm.display, gc,  2, LineSolid, CapButt, JoinRound);
	XDrawString(wm.display, parent_win->win, gc, 20, 20, child->title, strlen(child->title));

	Pixmap exit_icon, mask;
	XpmAttributes xpm_attr;
	XpmCreatePixmapFromData(wm.display, parent_win->win, exit_icon_xpm, &exit_icon, &mask, &xpm_attr);
	GC gc2;
	XGCValues values2;
       	gc2 = XCreateGC(wm.display, parent_win->win, None, &values2);
	XSetClipMask(wm.display, gc2, mask);
	// XcopyArea() server-side should be faster than XPutImage() client-side 
	XSetClipOrigin(wm.display, gc2, 600-xpm_attr.width, 0);
	XCopyArea(wm.display, exit_icon, parent_win->win, gc2, 0, 0, xpm_attr.width, xpm_attr.height, 600-xpm_attr.width, 0);

	return parent_win;
}

void my_keypress(char *keycode) {
	//printf("Key Pressed: %s\n", keycode);
}

void my_keyrelease(char *keycode) {
	int i, count;
	struct wm_window *windows, *parent_win;
	printf("Key Released: %s\n", keycode);
	if(strcmp(keycode, "t") == 0) {
		if(!fork())
			//execve("/usr/bin/xterm", NULL, NULL);
			system("/usr/bin/xterm");
	}
	else if(strcmp(keycode, "l") == 0) {
		count = wm_get_toplevel_windows(&wm, &windows);
		printf("\nwindow list (%d):\n", count);
		for(i=0 ; i<count ; i++)
			printf("win%d: %s - %s\n", i, windows[i].title, windows[i].icon);
		printf("\n");
	}
	else if(strcmp(keycode, "x") == 0)
		exit(0);
}

void my_draw(struct wm_window *win) {
	struct wm_window *parent_win;
	wm_window_show(&wm, win);
	parent_win = create_parent_window(win);
	wm_reparent(&wm, win, parent_win, 5, 25);
}



int main() {
	wm_new(&wm, "MyWM");
	if(!wm_init(&wm)) {
		printf("Error.");
		return 0;
	}
	wm_get_screen_resolution(&wm, &screen_width, &screen_height);
	printf("Screen: %dx%d\n", screen_width, screen_height);
	wm_set_background_color(&wm, 0, 255, 255);
	
	/*wm_set_cursor(&wm, CURSOR_DEFAULT);*/
	wm_set_cursor(&wm, CURSOR_ARROW);

	wm.key_press = my_keypress;
	wm.key_release = my_keyrelease;
	wm.draw = my_draw;

	wm_event_loop(&wm);

	wm_close(&wm);
	return 0;
}

