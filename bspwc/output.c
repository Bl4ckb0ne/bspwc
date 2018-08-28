#include "bspwc/output.h"

void output_destroy_notify(struct wl_listener* listener, void* data)
{
	struct output* output = wl_container_of(listener, output, destroy);

	wlr_output_layout_remove(output->server->output_layout, output->wlr_output);

	wl_list_remove(&output->link);
	wl_list_remove(&output->destroy.link);
	wl_list_remove(&output->frame.link);

	free(output);
}

void output_frame_notify(struct wl_listener* listener, void* data)
{
	struct output *output = wl_container_of(listener, output, frame);
	struct backend *backend = output->server->backend;
	struct wlr_output *wlr_output = data;
	struct wlr_renderer *renderer = wlr_backend_get_renderer(wlr_output->backend);

	// Render loop
	wlr_output_make_current(wlr_output, NULL);
	wlr_renderer_begin(renderer, wlr_output->width, wlr_output->height);

		float color[4] = {0.23f, 0.26f, 0.32f, 1.0f};
		wlr_renderer_clear(renderer, color);

		render_tree(output->desktop->root);

		wlr_output_swap_buffers(wlr_output, NULL, NULL);
	wlr_renderer_end(renderer);
}

void new_output_notify(struct wl_listener* listener, void* data)
{
	struct backend* backend = wl_container_of(listener, backend, new_output);
	struct wlr_output* wlr_output = data;

	if (!wl_list_empty(&wlr_output->modes))
	{
		struct wlr_output_mode* mode = wl_container_of(wlr_output->modes.prev, mode, link);
		wlr_output_set_mode(wlr_output, mode);
	}

	struct output* output = calloc(1, sizeof(struct output));
	if (output == NULL)
	{
		wlr_log(WLR_ERROR, "Failed to create output %s", wlr_output->name);
	}

	wlr_log(WLR_DEBUG, "Output '%s' added", wlr_output->name);
	wlr_log(WLR_DEBUG, "%s %s", wlr_output->make, wlr_output->model);
	wlr_log(WLR_DEBUG, "%s %"PRId32"mm x %"PRId32"mm", wlr_output->serial, wlr_output->phys_width, wlr_output->phys_height);

	clock_gettime(CLOCK_MONOTONIC, &output->last_frame);
	output->server = backend->server;
	output->wlr_output = wlr_output;

	wl_list_insert(&backend->outputs, &output->link);

	output->destroy.notify = output_destroy_notify;
	wl_signal_add(&wlr_output->events.destroy, &output->destroy);

	output->frame.notify = output_frame_notify;
	wl_signal_add(&wlr_output->events.frame, &output->frame);

	// TODO : make output position rearrangeable
	wlr_output_layout_add_auto(output->server->output_layout, output->wlr_output);

	wlr_output_create_global(wlr_output);

	output->desktop = create_desktop(output);
	if (output->desktop == NULL)
	{
		wlr_log(WLR_ERROR, "Failed to create default desktop for %s", wlr_output->name);
	}
}
