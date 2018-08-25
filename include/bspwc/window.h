#ifndef WINDOW_H
#define WINDOW_H

#include <wlr/types/wlr_xdg_shell_v6.h>

#include "bspwc/desktop.h"
#include "bspwc/xdg_surface_v6.h"

enum window_type
{
	NONE,
	WL_SHELL,
	XDG_SHELL,
	XDG_SHELL_V6,
	XWAYLAND,
};

struct window
{
	char* title;
	double x, y;
	uint32_t width, height;
	float alpha;

	struct desktop *desktop;

	enum window_type type;
	union
	{
		struct wlr_xdg_surface_v6 *wlr_xdg_surface_v6;
	};

	union
	{
		struct xdg_surface_v6 *xdg_surface_v6;
	};

	struct wlr_surface* wlr_surface;

	struct wl_listener new_subsurface;

	struct wl_signal event_unmap;
	struct wl_signal event_destroy;
};

struct window* create_window();

void resize_window(struct window *window, int width, int height);

#endif // WINDOW_H
