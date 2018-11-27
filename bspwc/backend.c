#include "bspwc/backend.h"

struct backend* create_backend(struct server* server)
{
	wlr_log(WLR_INFO, "Creating bspwc backend");

	struct backend* backend = calloc(1, sizeof(struct backend));
	if (backend == NULL)
	{
		wlr_log(WLR_ERROR, "Could not create backend");
		return NULL;
	}

	backend->server = server;

	backend->wlr_backend = wlr_backend_autocreate(server->display, NULL);
	assert(backend->wlr_backend);

	backend->wlr_compositor = wlr_compositor_create(
			server->display,
			wlr_backend_get_renderer(backend->wlr_backend)
		);
	backend->wlr_gamma_control_manager = wlr_gamma_control_manager_create(server->display);
	backend->wlr_screenshooter = wlr_screenshooter_create(server->display);
	backend->wlr_gtk_primary_selection_device_manager = wlr_gtk_primary_selection_device_manager_create(server->display);
	backend->wlr_idle = wlr_idle_create(server->display);
	backend->wlr_idle_inhibit = wlr_idle_inhibit_v1_create(server->display);
	backend->wlr_linux_dmabuf = wlr_linux_dmabuf_v1_create(server->display, wlr_backend_get_renderer(backend->wlr_backend));

	wl_list_init(&backend->outputs);

	// wlroots's shells
	backend->wlr_xdg_shell_v6 = wlr_xdg_shell_v6_create(server->display);

	// Wire listeners
	backend->new_output.notify = new_output_notify;
	wl_signal_add(&backend->wlr_backend->events.new_output, &backend->new_output);

	backend->new_xdg_shell_v6.notify = handle_xdg_shell_v6_surface;
	wl_signal_add(&backend->wlr_xdg_shell_v6->events.new_surface, &backend->new_xdg_shell_v6);

	return backend;
}

void destroy_backend(struct backend* backend)
{
	wlr_backend_destroy(backend->wlr_backend);

	free(backend);
}

void render_surface(struct wlr_output *wlr_output,
		struct wlr_surface *wlr_surface, const int x, const int y)
{
	if (!wlr_surface_has_buffer(wlr_surface))
	{
		return;
	}

	struct wlr_renderer *wlr_renderer = wlr_backend_get_renderer(
			wlr_output->backend);

	struct wlr_box render_box = {
			.x = x,
			.y = y,
			.width = wlr_surface->current.width,
			.height = wlr_surface->current.height
		};

	float matrix[16];
	wlr_matrix_project_box(matrix, &render_box, wlr_surface->current.transform,
			0, wlr_output->transform_matrix);

	struct wlr_texture *wlr_texture = wlr_surface_get_texture(wlr_surface);
	wlr_render_texture_with_matrix(wlr_renderer, wlr_texture, matrix, 1.0f);

	struct wlr_subsurface *wlr_subsurface;
	wl_list_for_each(wlr_subsurface, &wlr_surface->subsurfaces, parent_link)
	{
		struct wlr_surface_state *wlr_subsurface_state =
				&wlr_subsurface->surface->current;

		int pos_x = x + wlr_subsurface_state->dx;
		int pos_y = y + wlr_subsurface_state->dy;
		render_surface(wlr_output, wlr_subsurface->surface, pos_x, pos_y);
	}
}
