#ifndef SHELL_XDG_SURFACE_V6_H
#define SHELL_XDG_SURFACE_V6_H

#include <assert.h>
#include <wayland-server.h>
#include <wlr/types/wlr_xdg_shell_v6.h>
#include <wlr/util/log.h>

#include "bspwc/server.h"
#include "bspwc/window.h"

struct xdg_surface_v6
{
	struct window *window;

	struct wl_listener surface_commit;

	struct wl_listener destroy;
	struct wl_listener new_popup;
	struct wl_listener map;
	struct wl_listener unmap;
	struct wl_listener request_move;
	struct wl_listener request_resize;
	struct wl_listener request_maximize;
	struct wl_listener request_fullscreen;

	uint32_t pending_move_resize_configure_serial;
};

struct xdg_popup_v6
{
	struct xdg_surface_v6 *xdg_surface_v6;

	struct wlr_xdg_popup_v6 *wlr_popup;

	struct wl_listener destroy;
	struct wl_listener map;
	struct wl_listener unmap;
	struct wl_listener new_popup;
};

void handle_xdg_popup_v6_destroy(struct wl_listener *listener, void *data);
void handle_xdg_popup_v6_new_popup(struct wl_listener *listener, void *data);
void handle_xdg_popup_v6_map(struct wl_listener *listener, void *data);
void handle_xdg_popup_v6_unmap(struct wl_listener *listener, void *data);

struct xdg_popup_v6 *create_xdg_popup_v6(struct xdg_surface_v6 *xdg_surface_v6,
		struct wlr_xdg_popup_v6 *wlr_popup);


void handle_xdg_surface_v6_commit(struct wl_listener *listener, void *data);
void handle_xdg_surface_v6_destroy(struct wl_listener *listener, void *data);
void handle_xdg_surface_v6_new_popup(struct wl_listener *listener, void *data);
void handle_xdg_surface_v6_map(struct wl_listener *listener, void *data);
void handle_xdg_surface_v6_unmap(struct wl_listener *listener, void *data);
void handle_xdg_surface_v6_request_move(struct wl_listener *listener,
		void *data);
void handle_xdg_surface_v6_request_resize(struct wl_listener *listener,
		void *data);
void handle_xdg_surface_v6_request_maximize(struct wl_listener *listener,
		void *data);
void handle_xdg_surface_v6_request_fullscreen(struct wl_listener *listener,
		void *data);
void handle_xdg_shell_v6_surface(struct wl_listener *listener, void *data);

struct wlr_box get_size_wlr_xdg_surface_v6(struct wlr_xdg_surface_v6
		*wlr_xdg_surface_v6);
void resize_xdg_shell_v6(struct window *window, uint32_t initial_width,
		uint32_t initial_height);

#endif // SHELL_XDG_SURFACE_V6_H
