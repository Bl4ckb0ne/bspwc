#include "bspwc/tree.h"

struct node* node_create()
{
	wlr_log(L_DEBUG, "Creating node");
	struct node* node = calloc(1, sizeof(struct node));

	node->window = NULL;

	node->parent = NULL;
	node->left = NULL;
	node->right = NULL;

	return node;
}

void node_destroy(struct node* node)
{
	wlr_log(L_DEBUG, "Destroying node");
	if (node->left != NULL)
	{
		node_destroy(node->left);
	}

	if (node->right != NULL)
	{
		node_destroy(node->right);
	}

	free(node->window);
	free(node);
}

void insert(enum insert_mode mode, struct node* root, struct node* child)
{
	// TODO : verbose node insertion

	if (root->parent == NULL)
	{
		if ((root->left == NULL) && (root->right == NULL))
		{
			struct node* new_child = node_create();
			new_child->parent = root;
			new_child->window = root->window;
			root->window = NULL;

			if (mode == RIGHT)
			{
				root->left = new_child;
				root->right = child;
			}
			else if (mode == LEFT)
			{
				root->left = child;
				root->right = new_child;
			}
		}
		else if ((mode == RIGHT) && (root->right != NULL))
		{
			insert(mode, root->right, child);
		}
		else if ((mode == LEFT) && (root->right != NULL))
		{
			insert(mode, root->left, child);
		}
	}
	else
	{
		wlr_log(L_ERROR, "TODO");
	}
}
