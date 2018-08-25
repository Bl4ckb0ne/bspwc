#include <stdbool.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include <wlr/util/log.h>

#include "bspwc/config.h"

struct config *create_config()
{
	wlr_log(WLR_DEBUG, "Creating config");

	struct config *config = malloc(sizeof(struct config));

	config->mode = MANUAL;
	config->polarity = LEFT;

	return config;
}

void destroy_config(struct config *config)
{
	free(config);
}

// config file is executed.
bool load_config_file(const char* file)
{
	wlr_log(WLR_INFO, "Loading config file %s", file);
	int status;
	if (fork() == 0)
	{
		execl(file, file, NULL);
		exit(EXIT_FAILURE);
	}

	// Wait for the subprocess to finish and get the status, which contains
	// the exit status of the subprocess. You can access it using the
	// WIFEXITED and WEXITSTATUS macros.
	wait(&status);
	if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS)
	{
		return true;
	}

	return false;
}
