#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>

enum insertion_mode
{
	AUTOMATIC,
	MANUAL
};

enum insertion_polarity
{
	LEFT,
	RIGHT
};

enum insertion_split
{
	VERTICAL,
	HORIZONTAL
};

struct config
{
	enum insertion_mode mode;
	enum insertion_polarity polarity;
	enum insertion_split split;
};

struct config *create_config();
void destroy_config(struct config *config);

bool load_config_file(const char *file);

#endif // CONFIG_H
