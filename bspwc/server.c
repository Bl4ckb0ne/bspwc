#include "bspwc/server.h"

// TODO : Move to a proper place
static int read_events(int fd, uint32_t mask, void* data)
{
	const size_t BUFFER_SIZE = 128u;
	int ret = 0;

	// Get a pointer to the server
	struct server* s = data;

	int data_socket = accept(s->socket, NULL, NULL);
	if (data_socket == -1)
	{
		wlr_log(WLR_ERROR, "Failed to accept on socket %s", s->socket_name);
		return 1;
	}

	char buffer[BUFFER_SIZE];

	// TODO: Is it blocking?
	ret = read(data_socket, buffer, BUFFER_SIZE);
	if (ret == -1)
	{
		wlr_log(WLR_ERROR, "Failed to read on socket %s", s->socket_name);
		return 1;
	}
	buffer[BUFFER_SIZE - 1] = 0;

	wlr_log(WLR_INFO, "bspwc read : %s", buffer);

	return ret;
}

bool init_server(struct server* server)
{
	wlr_log(WLR_INFO, "Initializing bspwc server");

	server->output_layout = wlr_output_layout_create();

	// Create the display wlroots will render on
	server->display = wl_display_create();
	assert(server->display);

	// Init wayland's shared memory
	wl_display_init_shm(server->display);

	server->event_loop = wl_display_get_event_loop(server->display);
	assert(server->event_loop);

	// Create wlroots backend
	server->backend = create_backend(server);
	if (server->backend == NULL)
	{
		wlr_log(WLR_ERROR, "Failed to create server's backend");
		return false;
	}

	// Create wlroots input adaptor
	server->input = create_input(server);
	if (server->input == NULL)
	{
		wlr_log(WLR_ERROR, "Failed to create server's input");
		return false;
	}


	return true;
}

bool config_server(struct server* server)
{
	wlr_log(WLR_INFO, "Configuring bspwm server");

	// Create communication socket for bspc
	if (server->socket_name == NULL)
	{
		server->socket_name = malloc(strlen(BSPWC_DEFAULT_SOCKET) + 1);
		strcpy(server->socket_name, BSPWC_DEFAULT_SOCKET);
	}

	struct sockaddr_un sock;
	memset(&sock, 0, sizeof(struct sockaddr_un));

	sock.sun_family = AF_UNIX;
	strncpy(sock.sun_path, server->socket_name, sizeof(sock.sun_path) - 1);

	if ((server->socket = socket(AF_UNIX, SOCK_STREAM, 0)) == -1)
	{
		wlr_log(WLR_ERROR, "Failed to create socket %s", server->socket_name);
		return false;
	}

	unlink(server->socket_name);

	int ret = bind(server->socket, (const struct sockaddr *) &sock, sizeof(struct sockaddr_un));
	if (ret == -1)
	{
		wlr_log(WLR_ERROR, "Failed to bind to socket %s", server->socket_name);
		return false;
	}

	ret = listen(server->socket, SOMAXCONN);
	if (ret == -1)
	{
		wlr_log(WLR_ERROR, "Failed to listen on socket %s", server->socket_name);
		return false;
	}

	wlr_log(WLR_INFO, "BSPWM socket setup to %s", server->socket_name);

	// Create event listener for bspwm's socket
	server->event_source = wl_event_loop_add_fd(
			server->event_loop,
			server->socket,
			WL_EVENT_READABLE,
			read_events,
			&server
		);

	if (!server->event_source)
	{
		wlr_log(WLR_ERROR, "Failed to create input event on event loop");
		return false;
	}

	// Init default behaviors
	server->config = create_config();

	return true;
}

bool start_server(struct server* server)
{
	wlr_log(WLR_INFO, "Starting bspwc");

	// Wayland display socket
	const char* wl_socket = wl_display_add_socket_auto(server->display);
	if (!wl_socket)
	{
		wlr_log(WLR_ERROR, "Unable to open wayland socket");
		return false;
	}

	if (!wlr_backend_start(server->backend->wlr_backend))
	{
		wlr_log(WLR_ERROR, "Failed to start bspwc backend");
		return false;
	}

	setenv("WAYLAND_DISPLAY", wl_socket, true);
	wlr_log(WLR_INFO, "Running bspwc on wayland display '%s'", getenv("WAYLAND_DISPLAY"));

	return true;
}

bool terminate_server(struct server* server)
{
	wlr_log(WLR_INFO, "Terminating bspwc");

	destroy_config(server->config);

	wlr_output_layout_destroy(server->output_layout);
	destroy_input(server->input);
	destroy_backend(server->backend);

	wl_event_source_remove(server->event_source);
	wl_display_destroy(server->display);

	return true;
}

struct output *get_current_output(const struct server *server)
{
	wlr_log(WLR_DEBUG, "Getting the current output");

	const struct input *input = server->input;
	// Get desktop under the cursor
	struct wlr_output* wlr_output = wlr_output_layout_output_at(
			server->output_layout, input->cursor->wlr_cursor->x,
			input->cursor->wlr_cursor->y);

	if (wlr_output == NULL)
	{
		wlr_log(WLR_ERROR, "Failed to get active wlr_output");
		return false;
	}

	// Find the right output
	struct output* output = NULL;
	wl_list_for_each(output, &server->backend->outputs, link)
	{
		if (output->wlr_output == wlr_output)
		{
			return output;
		}
	}

	return NULL;
}
