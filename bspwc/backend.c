#include "bspwc/backend.h"

struct backend* create_backend(struct server* server)
{
	wlr_log(L_INFO, "Creating bspwc backend");

	struct backend* backend = calloc(1, sizeof(struct backend));
	if (backend == NULL)
	{
		wlr_log(L_ERROR, "Could not create backend");
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
	backend->wlr_primary_selection_device_manager = wlr_primary_selection_device_manager_create(server->display);
	backend->wlr_idle = wlr_idle_create(server->display);
	backend->wlr_idle_inhibit = wlr_idle_inhibit_v1_create(server->display);
	backend->wlr_linux_dmabuf = wlr_linux_dmabuf_create(server->display, wlr_backend_get_renderer(backend->wlr_backend));

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

void render_surface(struct wlr_output* wlr_output, struct wlr_surface* surface, const int x, const int y)
{
	if (!wlr_surface_has_buffer(surface))
	{
		return;
	}

	struct wlr_renderer* renderer = wlr_backend_get_renderer(wlr_output->backend);

	struct wlr_box render_box = {
			.x = x,
			.y = y,
			.width = surface->current->width,
			.height = surface->current->height
		};

	float matrix[16];
	wlr_matrix_project_box(
			matrix,
			&render_box,
			surface->current->transform,
			0,
			wlr_output->transform_matrix
		);

  struct wlr_texture *texture = wlr_surface_get_texture(surface);
	wlr_render_texture_with_matrix(renderer, texture, matrix, 1.0f);

	struct wlr_subsurface* subsurface;
	wl_list_for_each(subsurface, &surface->subsurfaces, parent_link)
	{
		struct wlr_surface_state *state = subsurface->surface->current;
		int sx = state->subsurface_position.x;
		int sy = state->subsurface_position.y;

		render_surface(wlr_output, subsurface->surface, x + sx, y + sy);
	}
}

bool insert_window_to_backend(struct backend* backend, struct window* window)
{
	struct server* server = backend->server;
	struct input* input = server->input;

	// Insert on active desktop
	struct wlr_output* wlr_output = wlr_output_layout_output_at(
			server->output_layout,
			input->cursor->wlr_cursor->x,
			input->cursor->wlr_cursor->y
		);

	if (wlr_output == NULL)
	{
		wlr_log(L_ERROR, "Inserting window %s in an empty layout", window->title);
		return false;
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
		wlr_log(L_ERROR, "Failed to found output to insert window %s", window->title);
		return false;
	}

	return insert_window_to_desktop(output->desktop, window);
}
