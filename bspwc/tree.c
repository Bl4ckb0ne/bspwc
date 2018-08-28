#include "bspwc/tree.h"

struct node *create_node()
{
	wlr_log(WLR_DEBUG, "Creating node");
	struct node *node = malloc(sizeof(struct node));

	node->window = NULL;

	node->parent = NULL;
	node->left = NULL;
	node->right = NULL;

	return node;
}

void destroy_node(struct node *node)
{
	wlr_log(WLR_DEBUG, "Destroying node");
	if (node->left != NULL)
	{
		destroy_node(node->left);
	}

	if (node->right != NULL)
	{
		destroy_node(node->right);
	}

	destroy_window(node->window);

	free(node);
}

bool insert_node(const struct server *server, struct node **root,
		struct node *child)
{
	const struct config *config = server->config;
	const struct output *output = get_current_output(server);

	if (config->mode == AUTOMATIC)
	{
		wlr_log(WLR_INFO, "Automatic insertion mode is not implemented");
		return false;
	}

	if (*root == NULL)
	{
		wlr_log(WLR_DEBUG, "Inserting %p at root", (void*)child);

		*root = child;

		// TODO: padding
		position_window((*root)->window, 0, 0);
		resize_window((*root)->window, output->wlr_output->width,
				output->wlr_output->height);
	}
	else
	{
		wlr_log(WLR_DEBUG, "Inserting %p into %p", (void*)child, (void*)root);
		wlr_log(WLR_INFO, "TODO");
		/*
		struct node* other_child = create_node();

		// copy data
		// remove data from root

		if (config->polarity == LEFT)
		{
			root->left = child;
			root->right = other_child;
		}
		else // config->polarity == RIGHT
		{
			root->right = child;
			root->left = other_child;
		}

		other_child->parent = root;
		child->parent = root;
		*/
	}

	return true;
}

void render_tree(const struct node *root)
{
	if (root == NULL)
	{
		return;
	}

	if (root->window != NULL)
	{
		wlr_log(WLR_INFO, "Rendering %p", root->window);
		render_window(root->window);
	}

	if (root->left != NULL)
	{
		render_tree(root->left);
	}

	if (root->right != NULL)
	{
		render_tree(root->right);
	}
}
