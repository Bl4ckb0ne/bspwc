#include "bspwc/window.h"

struct window* create_window()
{
	struct window* window = calloc(1, sizeof(struct window));
	if (window == NULL)
	{
		return NULL;
	}

	window->desktop = NULL;
	window->alpha = 1.0f;

	wl_signal_init(&window->event_unmap);
	wl_signal_init(&window->event_destroy);

	return window;
}
