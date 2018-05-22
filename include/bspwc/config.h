#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>

// Might not be the best place for this
enum insert_mode
{
	LEFT,
	RIGHT,
};

bool load_config_file(const char* file);

#endif // CONFIG_H
