#include <stdbool.h>

#include <wlr/util/log.h>

#include "bspwc/config.h"

// config file is executed.
// TODO: find a more explicative name?
bool load_config_file(const char* file)
{
	wlr_log(L_INFO, "Loading config file %s", file);

	return true;
}

