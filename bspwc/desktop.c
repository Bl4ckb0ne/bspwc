#include "bspwc/desktop.h"

struct desktop* create_desktop(struct output* output)
{
	wlr_log(L_DEBUG, "Creating desktop for %s", output->wlr_output->name);

	struct desktop* desktop = calloc(1, sizeof(struct desktop));
	if (desktop == NULL)
	{
		wlr_log(L_ERROR, "Failed to allocate desktop");
		return NULL;
	}

	desktop->output = output;

	snprintf(desktop->name, sizeof(desktop->name), "%s", DEFAULT_DESKTOP_NAME);

	// TODO: implement multi desktop
	desktop->next = NULL;
	desktop->previous = NULL;

	desktop->root = node_create();

	return desktop;
}
void destroy_desktop(struct desktop* desktop)
{
	wlr_log(L_DEBUG, "Destroying desktop %s", desktop->name);

	free(desktop);
}

bool insert_window_to_desktop(struct desktop* desktop, struct window* window)
{
	struct node* child = node_create();
	child->window = window;

	// wew that long
	// TODO: find a good way to get the insert mode
	return insert(desktop->output->server->insert_mode, desktop->root, child);
}
