#include "bspwc/window.h"

struct window* create_window(struct desktop* desktop)
{
	struct window* window = calloc(1, sizeof(struct window));
	if (window == NULL)
	{
		return NULL;
	}

	window->desktop = desktop;
	window->alpha = 1.0f;

	return window;
}
