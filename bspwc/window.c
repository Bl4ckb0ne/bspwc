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

void destroy_window(struct window *window)
{
	wlr_log(WLR_DEBUG, "Destroying window");

	// TODO: Destroy signals

	free(window);
}

void position_window(struct window *window, int x, int y)
{
	if (window == NULL)
	{
		wlr_log(WLR_ERROR, "Cannot resize null window");
		return;
	}

	wlr_log(WLR_INFO, "Window %s new position to %d, %d", window->title, x, y);

	window->x = x;
	window->y = y;
}

void resize_window(struct window *window, int width, int height)
{
	if (window == NULL)
	{
		wlr_log(WLR_ERROR, "Cannot resize null window");
		return;
	}

	wlr_log(WLR_INFO, "Window %s resize to %d, %d", window->title, width,
			height);

	window->width = width;
	window->height = height;
}
