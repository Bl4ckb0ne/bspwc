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

struct node *node_create();
void node_destroy(struct node *node);

bool insert(enum insert_mode mode, struct node* root, struct node* child);
bool is_leaf(struct node* node);

#endif // TREE_H
