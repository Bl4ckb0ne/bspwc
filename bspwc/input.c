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
	}

    return "UNKNOWN_DEVICE";
}

void handle_new_input(struct wl_listener* listener, void* data)
{
	struct input* input = wl_container_of(listener, input, new_input);

	struct wlr_input_device* device = data;
	enum wlr_input_device_type type = device->type;

	wlr_log(WLR_DEBUG, "New input device: %s (%d:%d) %s", device->name, device->vendor, device->product, device_type(type));

	if (type == WLR_INPUT_DEVICE_KEYBOARD)
	{
		wlr_log(WLR_INFO, "TODO : handle keyboard");
	}
	else if (WLR_INPUT_DEVICE_POINTER)
	{
		if (input->cursor == NULL)
		{
			input->cursor = create_cursor(input, device);
		}
	}
	else
	{
		wlr_log(WLR_INFO, "Device '%s' not implemented", device_type(type));
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
        free(input);
        return NULL;
    }

	input->cursor = NULL;

	input->new_input.notify = handle_new_input;
	wl_signal_add(&server->backend->wlr_backend->events.new_input,
            &input->new_input);

    wlr_log(WLR_DEBUG, "Input %p created", (void*)input);
	return input;
}

void destroy_input(struct input* input)
{
    wlr_log(WLR_DEBUG, "Destroying input %p", (void*)input);
    wlr_seat_destroy(input->seat);

	if (input->cursor != NULL)
	{
		destroy_cursor(input->cursor);
	}

	free(input);
}
