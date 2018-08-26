#ifndef TREE_H
#define TREE_H

#include <stdbool.h>

#include "bspwc/config.h"
#include "bspwc/window.h"

struct node
{
	struct window *window;

	struct node *parent;
	struct node *left;
	struct node *right;
};

struct node *create_node();
void destroy_node(struct node *node);

bool insert_node(const struct config *config, struct node* root,
		struct node* child);
bool is_leaf(struct node* node);

#endif // TREE_H
