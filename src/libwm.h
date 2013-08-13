#ifndef _LIBWM_H
#define _LIBWM_H

#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include <X11/XKBlib.h>
#include <X11/Xutil.h>
//#include <X11/Xatom.h>

struct wm_context{
        char *wm_name;
        Display *display;
        Window root_window;
	int screen;
	void (*key_press)(char *);
	void (*key_release)(char *);
};

struct wm_window{
	Window win;
	char *title;
	char *icon;
};

#define	CURSOR_DEFAULT	XC_X_cursor
#define	CURSOR_ARROW	XC_arrow
#define	CURSOR_CROSS	XC_cross
#define	CURSOR_BEAM	XC_xterm
#define	CURSOR_MOVE	XC_fleur
#define CURSOR_HAND	XC_hand2
#define	CURSOR_HORIZONTAL	XC_sb_h_double_arrow
#define	CURSOR_VERTICAL	XC_sb_v_double_arrow
#define	CURSOR_SZIE_BR	XC_bottom_right_corner
#define	CURSOR_SIZE_BL	XC_bottom_left_corner
#define	CURSOR_SIZE_TR	XC_top_right_corner
#define	CURSOR_SIZE_TL	XC_top_left_corner
#define	CURSOR_BUSY	XC_watch
#define	CURSOR_DRAG	XC_target
#define	CURSOR_NO	XC_pirate

void wm_new(struct wm_context *, char *);
int wm_init(struct wm_context *);
void wm_close();
void wm_get_screen_resolution(struct wm_context *, unsigned int *, unsigned int *);
void wm_set_background_color(struct wm_context *, unsigned short, unsigned short, unsigned short);
void wm_set_cursor(struct wm_context *, unsigned int);
void wm_event_loop(struct wm_context *);
unsigned int wm_get_windows(struct wm_context *, struct wm_window **);
void wm_move_window(struct wm_context *, struct wm_window *, unsigned int, unsigned int);
void wm_resize_window(struct wm_context *, struct wm_window *, unsigned int, unsigned int);
void wm_set_border_width(struct wm_context *, struct wm_window *, unsigned int);
void wm_set_border_color(struct wm_context *, struct wm_window *, unsigned short, unsigned short, unsigned short);
void wm_reparent(struct wm_context *, struct wm_window *, unsigned int, unsigned int);

#endif

