#include "bspwc/input.h"

const char *device_type(enum wlr_input_device_type type)
{
	switch (type)
	{
	case WLR_INPUT_DEVICE_KEYBOARD:
		return "keyboard";
	case WLR_INPUT_DEVICE_POINTER:
		return "pointer";
	case WLR_INPUT_DEVICE_TOUCH:
		return "touch";
	case WLR_INPUT_DEVICE_TABLET_TOOL:
		return "tablet tool";
	case WLR_INPUT_DEVICE_TABLET_PAD:
		return "tablet pad";
	case WLR_INPUT_DEVICE_SWITCH:
		return "switch";
	}

	return "UNKNOWN_DEVICE";
}

void handle_new_input(struct wl_listener *listener, void *data)
{
	struct input *input = wl_container_of(listener, input, new_input);

	struct wlr_input_device *device = data;
	enum wlr_input_device_type type = device->type;

	wlr_log(WLR_DEBUG, "New input device: %s (%s) %d:%d", device->name,
			device_type(type), device->vendor, device->product);

	uint32_t capabilities = 0;
	if (type == WLR_INPUT_DEVICE_KEYBOARD)
	{
		if (input->keyboard == NULL)
		{
			input->keyboard = create_keyboard(input, device);
		}

		capabilities |= WL_SEAT_CAPABILITY_KEYBOARD;
	}
	else if (WLR_INPUT_DEVICE_POINTER)
	{
		if (input->cursor == NULL)
		{
			input->cursor = create_cursor(input, device);
		}

		capabilities |= WL_SEAT_CAPABILITY_POINTER;
	}
	else
	{
		wlr_log(WLR_INFO, "Device %s (%s) not implemented",
				device->name, device_type(type));
	}

	wlr_seat_set_capabilities(input->seat, capabilities);
}

void handle_request_cursor(struct wl_listener *listener, void *data)
{
	wlr_log(WLR_DEBUG, "Input request cursor");
	struct input *input = wl_container_of(listener, input, request_cursor);
	struct wlr_seat_pointer_request_set_cursor_event *event = data;
	struct wlr_seat_client *focused_client =
		input->seat->pointer_state.focused_client;

	if (focused_client == event->seat_client)
	{
		wlr_cursor_set_surface(input->cursor->wlr_cursor, event->surface,
				event->hotspot_x, event->hotspot_y);
	}
}

struct input *create_input(struct server *server)
{
	wlr_log(WLR_DEBUG, "Creating input");
	assert(server);

	struct input *input = malloc(sizeof(struct input));
	if (input == NULL)
	{
		wlr_log(WLR_ERROR, "Failed to create input");
		return NULL;
	}

	input->server = server;

	input->seat = wlr_seat_create(server->display, INPUT_DEFAULT_SEAT_NAME);
	if (input->seat == NULL)
	{
		wlr_log(WLR_ERROR, "Failed to create wlr_seat");
		destroy_input(input);
		return NULL;
	}

	input->cursor = NULL;
	input->keyboard = NULL;

	input->new_input.notify = handle_new_input;
	wl_signal_add(&server->backend->wlr_backend->events.new_input,
			&input->new_input);

	input->request_cursor.notify = handle_request_cursor;
	wl_signal_add(&input->seat->events.request_set_cursor,
			&input->request_cursor);

	wlr_log(WLR_DEBUG, "Input %p created", (void*)input);
	return input;
}

void destroy_input(struct input *input)
{
	wlr_log(WLR_DEBUG, "Destroying input %p", (void*)input);
	wlr_seat_destroy(input->seat);

	if (input->cursor != NULL)
	{
		destroy_cursor(input->cursor);
	}
	if (input->keyboard != NULL)
	{
		destroy_keyboard(input->keyboard);
	}

	free(input);
	wlr_log(WLR_DEBUG, "Input destroyed");
}
