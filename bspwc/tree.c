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

bool insert(enum insert_mode mode, struct node* root, struct node* child)
{
	wlr_log(L_DEBUG, "Inserting %p", (void*)child);

	struct node* candidate = root;

	while (true)
	{
		wlr_log(L_DEBUG, "Candidate %p", (void*)candidate);
		// Real root case
		if (candidate->parent == NULL)
		{
			if ((candidate->left == NULL) && (candidate->right == NULL))
			{
				wlr_log(L_DEBUG, "Insert at root");
				break;
			}
		}

		if (mode == RIGHT)
		{
			if (candidate->right != NULL)
			{
				candidate = candidate->right;
				continue;
			}
			else
			{
				break;
			}
		}
		else if ((mode == LEFT) && (candidate->left != NULL))
		{
			if (candidate->left != NULL)
			{
				candidate = candidate->left;
				continue;
			}
			else
			{
				break;
			}
		}

		wlr_log(L_ERROR, "Error founding insertion candidate");
		return false;

	}

	struct node* new_child = node_create();
	new_child->window = root->window;
	new_child->parent = root;
	child->parent = root;
	root->window = NULL;

	if (mode == RIGHT)
	{
		wlr_log(L_DEBUG, "Inserting into right of %p", (void*)root);
		root->left = new_child;
		root->right = child;
	}
	else if (mode == LEFT)
	{
		wlr_log(L_DEBUG, "Inserting into left of %p", (void*)root);
		root->left = child;
		root->right = new_child;
	}

	return true;
}
