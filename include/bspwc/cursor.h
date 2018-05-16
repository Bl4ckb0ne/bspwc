#ifndef CURSOR_H
#define CURSOR_H

#include <wayland-server.h>
#include <wlr/types/wlr_cursor.h>
#include <wlr/types/wlr_input_device.h>
#include <wlr/types/wlr_xcursor_manager.h>

#include "bspwc/input.h"

struct cursor
{
	struct input* input;

	struct wlr_cursor* wlr_cursor;
	struct wlr_xcursor_manager* wlr_xcursor_manager;

	struct wl_listener motion;
	struct wl_listener motion_absolute;
	struct wl_listener button;
	struct wl_listener axis;
};

void handle_cursor_motion(struct wl_listener* listener, void* data);
void handle_cursor_motion_absolute(struct wl_listener* listener, void* data);
void handle_cursor_button(struct wl_listener* listener, void* data);
void handle_cursor_axis(struct wl_listener* listener, void* data);

struct cursor* create_cursor(struct input* input, struct wlr_input_device* device);
void destroy_cursor(struct cursor* cursor);

#endif // CURSOR_H
