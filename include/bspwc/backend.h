#ifndef BACKEND_H
#define BACKEND_H

#include <wayland-server.h>
#include <wlr/backend.h>
#include <wlr/types/wlr_compositor.h>
#include <wlr/types/wlr_gamma_control.h>
#include <wlr/types/wlr_idle.h>
#include <wlr/types/wlr_idle_inhibit_v1.h>
#include <wlr/types/wlr_linux_dmabuf_v1.h>
#include <wlr/types/wlr_primary_selection.h>
#include <wlr/types/wlr_screenshooter.h>
#include <wlr/types/wlr_server_decoration.h>
#include <wlr/types/wlr_xdg_shell.h>
#include <wlr/types/wlr_xdg_shell_v6.h>
#include <wlr/types/wlr_wl_shell.h>

#include "bspwc/output.h"
#include "bspwc/server.h"
#include "bspwc/window.h"
#include "bspwc/tree.h"

// Forward declaration for backend
struct window;

struct backend
{
	struct server *server;

	struct wlr_backend *wlr_backend;
	struct wlr_compositor *wlr_compositor;
	struct wlr_gamma_control_manager *wlr_gamma_control_manager;
	struct wlr_screenshooter *wlr_screenshooter;
	struct wlr_server_decoration_manager *wlr_server_decoration_manager;
	struct wlr_primary_selection_device_manager 
		*wlr_primary_selection_device_manager;
	struct wlr_idle *wlr_idle;
	struct wlr_idle_inhibit_manager_v1 *wlr_idle_inhibit;
	struct wlr_linux_dmabuf_v1 *wlr_linux_dmabuf;

	struct wlr_xdg_shell_v6 *wlr_xdg_shell_v6;

	struct wl_listener new_xdg_shell_v6;
	struct wl_listener new_output;

	struct wl_list outputs; // output::link
};

struct backend *create_backend(struct server *server);
void destroy_backend(struct backend *backend);

void render_surface(struct wlr_output *wlr_output, struct wlr_surface *surface,
		const int x, const int y);

#endif // BACKEND_H
