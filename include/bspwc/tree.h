#ifndef TREE_H
#define TREE_H

#include "bspwc/config.h"
#include "bspwc/window.h"

struct node
{
	struct window* window;

	struct node* parent;
	struct node* left;
	struct node* right;
};

struct node* node_create();
void node_destroy(struct node* node);

void insert(enum insert_mode mode, struct node* insertion, struct node* node);

#endif // TREE_H
