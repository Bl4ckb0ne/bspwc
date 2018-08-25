#include "bspwc/window.h"

struct window* create_window()
{
	wlr_log(WLR_DEBUG, "Creating new window");
	struct window *window = malloc(sizeof(struct window));
	if (window == NULL)
	{
		return NULL;
	}

	window->title = NULL;
	window->x = 0;
	window->y = 0;
	window->width = 0;
	window->height = 0;
	window->alpha = 1.0f;

	window->desktop = NULL;

	window->type = NONE;

	wl_signal_init(&window->event_unmap);
	wl_signal_init(&window->event_destroy);

	return window;
}

void resize_window(struct window *window, int width, int height)
{
	wlr_log(WLR_INFO, "Resizing window %s to %d, %d", window->title, width,
			height);
	window->width = width;
	window->height = height;
}
