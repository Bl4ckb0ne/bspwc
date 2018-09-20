#define _POSIX_C_SOURCE 200112L

#include <getopt.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <wayland-server.h>
#include <wlr/backend.h>
#include <wlr/util/log.h>

#include "bspwc/config.h"
#include "bspwc/server.h"

struct wl_display* display = NULL;

void sig_handler(int sig)
{
	const char *signal;

	switch (sig)
	{
		case SIGINT:
			signal = "SIGINT";
			break;
		case SIGTERM:
			signal = "SIGTERM";
			break;
		default:
			signal = "TODO";
			break;
	}

	wlr_log(WLR_INFO, "%s caugh, terminating display", signal);
	wl_display_terminate(display);
}

int main(int argc, char *argv[])
{
	static int verbose = 0, debug = 0;

	char* config_file = NULL;
	char* socket_name = NULL;

	struct server server = {0};

	const char* usage =
		"Usage: bspwc [option]\n"
		"\n"
		" -h, --help\t\tShow this message\n"
		" -c, --config\t\tSpecify a configuration file\n"
		" -s, --socket\t\tSpecify a socket to use\n"
		" -v, --version\t\tShow the version number and exits\n"
		" -V, --verbose\t\tEnable verbose output\n"
		" -d, --debug\t\tEnable debug output\n"
		"\n";

	static struct option long_options[] = {
		{"help", no_argument, NULL, 'h'},
		{"config", required_argument, NULL, 'c'},
		{"socket", optional_argument, NULL, 's'},
		{"version", no_argument, NULL, 'v'},
		{"verbose", no_argument, NULL, 'V'},
		{"debug", no_argument, NULL, 'd'},
		{0, 0, 0, 0}
	};

	int opt;
	while ((opt = getopt_long(argc, argv, "hc:s:vVd", long_options, NULL)) != -1)
	{
		switch (opt)
		{
			case 'h':
				fprintf(stdout, "%s", usage);
				exit(EXIT_SUCCESS);
				break;
			case 'c':
				config_file = malloc(strlen(optarg) + 1);
				strcpy(config_file, optarg);
				break;
			case 's':
				server.socket_name = malloc(strlen(optarg) + 1);
				strcpy(server.socket_name, optarg);
				break;
			case 'v':
				printf("%s\n", BSPWC_VERSION);
				exit(EXIT_SUCCESS);
				break;
			case 'V':
				verbose = 1;
				break;
			case 'd':
				debug = 1;
				break;
		}
	}

	if (debug)
	{
		wlr_log_init(WLR_DEBUG, NULL);
	}
	else if (verbose)
	{
		wlr_log_init(WLR_INFO, NULL);
	}
	else
	{
		wlr_log_init(WLR_ERROR, NULL);
	}

	if (init_server(&server) == false)
	{
		wlr_log(WLR_ERROR, "Failed to initialize bspwc server");
		exit(EXIT_FAILURE);
	}

	// Assign global display
	display = server.display;

	if (config_server(&server) == false)
	{
		wlr_log(WLR_ERROR, "Failed to configure bspwc server");
		terminate_server(&server);
		exit(EXIT_FAILURE);
	}

	// Start BSPWC
	if (!start_server(&server))
	{
		wlr_log(WLR_ERROR, "Failed to start server");
		terminate_server(&server);
		exit(EXIT_FAILURE);
	}

	// If no config_file is given, the default one is
	// $HOME/.config/bspwc/bspwcrc
	if (config_file == NULL)
	{
		char* home_dir = getenv("HOME");
		if(home_dir == NULL)
		{
			wlr_log(WLR_ERROR, "Failed to get HOME environment variable");
		}

		config_file = malloc(
			strlen(home_dir) + strlen(BSPWC_DEFAULT_CONFIG_FILE) + 1
		);
		config_file[0] = '\0';

		strcat(config_file, home_dir);
		strcat(config_file, BSPWC_DEFAULT_CONFIG_FILE);
	}

	if (!load_config_file(config_file))
	{
		wlr_log(WLR_ERROR, "Failed to load config file");
		terminate_server(&server);
		exit(EXIT_FAILURE);
	}

	signal(SIGINT, sig_handler);

	wl_display_run(server.display);

	terminate_server(&server);

	free(config_file);
	free(socket_name);

	return 0;
}
