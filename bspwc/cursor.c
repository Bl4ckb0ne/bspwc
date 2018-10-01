#include "bspwc/cursor.h"

void handle_cursor_motion(struct wl_listener* listener, void* data)
{
	struct cursor* cursor = wl_container_of(listener, cursor, motion);
	struct wlr_event_pointer_motion* event = data;

	wlr_cursor_move(cursor->wlr_cursor, event->device, event->delta_x, event->delta_y);
}

void handle_cursor_motion_absolute(struct wl_listener* listener, void* data)
{
	struct cursor* cursor = wl_container_of(listener, cursor, motion_absolute);
	struct wlr_event_pointer_motion_absolute* event = data;

	wlr_cursor_warp_absolute(cursor->wlr_cursor, event->device, event->x, event->y);
}

void handle_cursor_button(struct wl_listener* listener, void* data)
{}

void handle_cursor_axis(struct wl_listener* listener, void* data)
{}

struct cursor* create_cursor(struct input* input, struct wlr_input_device* device)
{
	struct cursor* cursor = calloc(1, sizeof(struct cursor));
	if (cursor == NULL)
	{
		wlr_log(WLR_ERROR, "Failed to create cursor");
		return NULL;
	}

	cursor->input = input;

	cursor->wlr_cursor = wlr_cursor_create();
	if (cursor->wlr_cursor == NULL)
	{
		wlr_log(WLR_ERROR, "Failed to create cursor's wlr_cursor");
		free(cursor);
	}
	wlr_cursor_attach_input_device(cursor->wlr_cursor, device);

	struct wlr_output_layout* output_layout = input->server->output_layout;
	wlr_cursor_attach_output_layout(cursor->wlr_cursor, output_layout);

	cursor->wlr_xcursor_manager = wlr_xcursor_manager_create("default", 24);
	if (cursor->wlr_xcursor_manager == NULL)
	{
		wlr_log(WLR_ERROR, "Failed to load left_ptr cursor");
		free(cursor);
		return NULL;
	}

	wlr_xcursor_manager_load(cursor->wlr_xcursor_manager, 1);
	wlr_xcursor_manager_set_cursor_image(cursor->wlr_xcursor_manager, "left_ptr", cursor->wlr_cursor);

	struct wlr_box* layout_box = wlr_output_layout_get_box(output_layout, NULL);
	wlr_cursor_warp(cursor->wlr_cursor, NULL, layout_box->width / 2 , layout_box->height / 2);

	// Setup callbacks
	wl_signal_add(&cursor->wlr_cursor->events.motion, &cursor->motion);
	cursor->motion.notify = handle_cursor_motion;

	wl_signal_add(&cursor->wlr_cursor->events.motion_absolute, &cursor->motion_absolute);
	cursor->motion_absolute.notify = handle_cursor_motion_absolute;

	wl_signal_add(&cursor->wlr_cursor->events.button, &cursor->button);
	cursor->button.notify = handle_cursor_button;

	wl_signal_add(&cursor->wlr_cursor->events.axis, &cursor->axis);
	cursor->axis.notify = handle_cursor_axis;

	// Let wlr_seat know we have a pointer
	uint32_t caps = WL_SEAT_CAPABILITY_POINTER;
	wlr_seat_set_capabilities(input->seat, caps);

	return cursor;
}

void destroy_cursor(struct cursor* cursor)
{
	wlr_xcursor_manager_destroy(cursor->wlr_xcursor_manager);
	wlr_cursor_destroy(cursor->wlr_cursor);
	free(cursor);
}
