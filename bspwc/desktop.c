#include "bspwc/desktop.h"

struct desktop* create_desktop(struct output *output)
{
	wlr_log(WLR_DEBUG, "Creating desktop for %s", output->wlr_output->name);

	struct desktop* desktop = calloc(1, sizeof(struct desktop));
	if (desktop == NULL)
	{
		return NULL;
	}

	desktop->output = output;

	snprintf(desktop->name, sizeof(desktop->name), "%s", DEFAULT_DESKTOP_NAME);

	// TODO: implement multi desktop
	desktop->next = NULL;
	desktop->previous = NULL;

	desktop->root = NULL;

	return desktop;
}
void destroy_desktop(struct desktop *desktop)
{
	wlr_log(WLR_DEBUG, "Destroying desktop %s", desktop->name);

	free(desktop);
}
