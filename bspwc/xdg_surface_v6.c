#include "bspwc/xdg_surface_v6.h"

void handle_surface_commit(struct wl_listener* listener, void* data)
{}

void handle_destroy(struct wl_listener* listener, void* data)
{}

void handle_new_popup(struct wl_listener* listener, void* data)
{}

void handle_map(struct wl_listener* listener, void* data)
{}

void handle_unmap(struct wl_listener* listener, void* data)
{}

void handle_request_move(struct wl_listener* listener, void* data)
{}

void handle_request_resize(struct wl_listener* listener, void* data)
{}

void handle_request_maximize(struct wl_listener* listener, void* data)
{}

void handle_request_fullscreen(struct wl_listener* listener, void* data)
{}

void handle_xdg_shell_v6_surface(struct wl_listener* listener, void* data)
{
    struct wlr_xdg_surface_v6* wlr_xdg_surface_v6 = data;
    assert(wlr_xdg_surface_v6->role != WLR_XDG_SURFACE_V6_ROLE_NONE);

    if (wlr_xdg_surface_v6->role == WLR_XDG_SURFACE_V6_ROLE_POPUP)
    {
        wlr_log(L_INFO, "New xdg_v6 popup");
        return;
    }

    struct backend* backend = wl_container_of(listener, backend, new_xdg_shell_v6);
	struct server* server = backend->server;
	struct input* input = server->input;

    wlr_log(
			L_INFO,
			"New xdg_v6 toplevel: title=%s, app_id=%s",
			wlr_xdg_surface_v6->toplevel->title,
			wlr_xdg_surface_v6->toplevel->app_id
        );
    wlr_xdg_surface_v6_ping(wlr_xdg_surface_v6);

    struct xdg_surface_v6* surface = calloc(1, sizeof(struct xdg_surface_v6));

    surface->surface_commit.notify = handle_surface_commit;
    wl_signal_add(&wlr_xdg_surface_v6->surface->events.commit, &surface->surface_commit);

    surface->destroy.notify = handle_destroy;
    wl_signal_add(&wlr_xdg_surface_v6->events.destroy, &surface->destroy);

    surface->new_popup.notify = handle_new_popup;
    wl_signal_add(&wlr_xdg_surface_v6->events.new_popup, &surface->new_popup);

    surface->map.notify = handle_map;
    wl_signal_add(&wlr_xdg_surface_v6->events.map, &surface->map);

    surface->unmap.notify = handle_unmap;
    wl_signal_add(&wlr_xdg_surface_v6->events.unmap, &surface->unmap);

    surface->request_move.notify = handle_request_move;
    wl_signal_add(&wlr_xdg_surface_v6->toplevel->events.request_move, &surface->request_move);

    surface->request_resize.notify = handle_request_resize;
    wl_signal_add(&wlr_xdg_surface_v6->toplevel->events.request_resize, &surface->request_resize);

    surface->request_maximize.notify = handle_request_maximize;
    wl_signal_add(&wlr_xdg_surface_v6->toplevel->events.request_maximize, &surface->request_maximize);

    surface->request_fullscreen.notify = handle_request_fullscreen;
    wl_signal_add(&wlr_xdg_surface_v6->toplevel->events.request_fullscreen, &surface->request_fullscreen);

	// Create xdg_v6 window
    struct window* window = create_window();
    if (window == NULL)
    {
		wlr_log(L_ERROR, "Failed to create window for %s", wlr_xdg_surface_v6->toplevel->title);
        free(surface);
        return;
    }

    window->type = XDG_SHELL_V6;
    window->wlr_xdg_surface_v6 = wlr_xdg_surface_v6;
    window->xdg_surface_v6 = surface;

	// Insert on active desktop
	struct wlr_output* wlr_output = wlr_output_layout_output_at(
			server->output_layout,
			input->cursor->wlr_cursor->x,
			input->cursor->wlr_cursor->y
		);

	if (wlr_output == NULL)
	{
		return;
	}

	// Find the right output
	struct output* output = NULL;
	wl_list_for_each(output, &backend->outputs, link)
	{
		if (output->wlr_output == wlr_output)
		{
			break;
		}
	}

	if (output == NULL)
	{
		wlr_log(L_ERROR, "Failed to found output to insert window");
		return;
	}

	if (insert_window(output, window) == false)
	{
		wlr_log(L_ERROR, "Failed to insert window into desktop");
	}
}
