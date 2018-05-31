#ifndef SURFACE_H
#define SURFACE_H

#include <wayland-server.h>

#include "bspwc/window.h"

struct xdg_surface_v6
{
	struct window* window;

	struct wl_listener destroy;
	struct wl_listener new_popup;
	struct wl_listener map;
	struct wl_listener unmap;
	struct wl_listener request_move;
	struct wl_listener request_resize;
	struct wl_listener request_maximize;
	struct wl_listener request_fullscreen;

	struct wl_listener surface_commit;

	uint32_t pending_move_resize_configure_serial;
};

#endif // SURFACE_H
