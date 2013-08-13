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

#include "libwm.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void wm_new(struct wm_context *cntx, char *name) {
	//strcpy(cntx->wm_name, name);
	cntx->wm_name = name;
	cntx->display = NULL;
	cntx->screen = 0;
	cntx->key_press = NULL;
	cntx->key_release = NULL;
}

int wm_init(struct wm_context* cntx) {
	if(cntx == NULL)
		return 0;

	cntx->display = XOpenDisplay(NULL);
	if(cntx->display == NULL)
		return 0;

	cntx->root_window = DefaultRootWindow(cntx->display);
	cntx->screen = DefaultScreen(cntx->display);


	XGrabKey(cntx->display,
		XKeysymToKeycode(cntx->display, XStringToKeysym("T")),
		Mod4Mask,
		cntx->root_window,
		True,
		GrabModeAsync,
		GrabModeAsync
	);

	XSelectInput(cntx->display, cntx->root_window, KeyPressMask | KeyReleaseMask);

	return 1;
}

void wm_close(struct wm_context* cntx) {
	XCloseDisplay(cntx->display);
}

void wm_get_screen_resolution(struct wm_context *cntx, unsigned int *width, unsigned int *height) {
	*width = DisplayWidth(cntx->display , cntx->screen);
	*height = DisplayHeight(cntx->display, cntx->screen);
}

void wm_set_background_color(struct wm_context *cntx, unsigned short red, unsigned short green, unsigned short blue) {
	XColor bg;
	/* X uses color scale from 0 -> 65535 */
	bg.red = red * 256; 
	bg.green = green * 256;
	bg.blue = blue * 256;
	unsigned long clr = BlackPixel(cntx->display, cntx->screen);
	//XParseColor(cntx->display, DefaultColormap(cntx->display, cntx->screen), "#00FF00", &bg);
	if(XAllocColor(cntx->display, DefaultColormap(cntx->display, cntx->screen), &bg)) 
		clr = bg.pixel;
	XSetWindowBackground(cntx->display, cntx->root_window, clr);
	XClearWindow(cntx->display, cntx->root_window);
	XFlush(cntx->display);
}

void wm_set_cursor(struct wm_context *cntx, unsigned int cursor) {
	Cursor xcursor = XCreateFontCursor(cntx->display, cursor);
	XDefineCursor(cntx->display, cntx->root_window, xcursor);
	XFlush(cntx->display);
}

void wm_event_loop(struct wm_context *cntx) {
	XEvent event;
	for(;;) {
		XNextEvent(cntx->display, &event);
		if(event.type == KeyPress && cntx->key_press != NULL) {
			(*(cntx->key_press))(XKeysymToString(XkbKeycodeToKeysym(cntx->display, event.xkey.keycode, 0, 0)));
		}
		else if(event.type == KeyRelease && cntx->key_release != NULL) {
			(*(cntx->key_release))(XKeysymToString(XkbKeycodeToKeysym(cntx->display, event.xkey.keycode, 0, 0)));
		}
	}
}

unsigned int wm_get_windows(struct wm_context *cntx, struct wm_window **windows) {
	Window root;
	Window parent;
	Window *children;
	XTextProperty prop, prop2;
	XWindowAttributes attr;
	unsigned int children_count;
	unsigned int viewable_count = 0;
	int i;
	char *win_name;

	if(!XQueryTree(cntx->display, cntx->root_window, &root, &parent, &children, &children_count)) {
		printf("Error!!\n");
	}
	*windows = (struct wm_window *)malloc(sizeof(struct wm_window)*children_count);
	for(i=0 ; i<children_count ; i++) {
		XGetWindowAttributes(cntx->display, children[i], &attr);
		if(attr.map_state != IsViewable)
			continue;
		viewable_count++;

		(*windows)[i].win = children[i];
		/*
		XFetchName(cntx->display, children[i], &win_name);
		(*windows)[i].title = win_name;
		*/
		XGetWMName(cntx->display, children[i], &prop);
		(*windows)[i].title = prop.value;
		/*XFree(win_name);*/
		XGetWMIconName(cntx->display, children[i], &prop2);
		(*windows)[i].icon = prop2.value;
	}

	return viewable_count;
}

void wm_move_window(struct wm_context *cntx, struct wm_window *win, unsigned int x, unsigned int y) {
	XMoveWindow(cntx->display, win->win, x, y);
}

void wm_resize_window(struct wm_context *cntx, struct wm_window *win, unsigned int w, unsigned int h) {
	XResizeWindow(cntx->display, win->win, w, h);
}

void wm_set_border_width(struct wm_context *cntx, struct wm_window *win, unsigned int w) {
	XSetWindowBorderWidth(cntx->display, win->win, w);
}

void wm_set_border_color(struct wm_context *cntx, struct wm_window *win, unsigned short r, unsigned short g, unsigned short b) {
	XColor border;
	/* X uses color scale from 0 -> 65535 */
	border.red = r * 256; 
	border.green = g * 256;
	border.blue = b * 256;
	unsigned long clr = BlackPixel(cntx->display, cntx->screen);
	if(XAllocColor(cntx->display, DefaultColormap(cntx->display, cntx->screen), &border))
		clr = border.pixel;
	XSetWindowBorder(cntx->display, win->win, clr);
}

void wm_reparent(struct wm_context *cntx, struct wm_window *win, unsigned int x, unsigned int y) {
	GC gc;
	XGCValues values;

	//int blackColor = BlackPixel(cntx->display, DefaultScreen(cntx->display));
	int blackColor = WhitePixel(cntx->display, DefaultScreen(cntx->display));
	Window parent_win = XCreateSimpleWindow(cntx->display, DefaultRootWindow(cntx->display), 0, 0, 600, 600, 0, blackColor, blackColor);
	XReparentWindow(cntx->display, win->win, parent_win, x, y);
	XMapWindow(cntx->display, parent_win);

	gc = XCreateGC(cntx->display, parent_win, None, &values);
	XSetLineAttributes(cntx->display, gc,  2, LineSolid, CapButt, JoinRound);
	XDrawString(cntx->display, parent_win, gc, 20, 20, "title", 5);

	XMoveWindow(cntx->display, parent_win, 20, 20);

	XFlush(cntx->display);
}

