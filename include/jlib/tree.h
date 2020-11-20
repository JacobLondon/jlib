#ifndef JLIB_TREE_H
#define JLIB_TREE_H

#include <jlib/list.h>

/**
 * A tree node, where a leaf's \a children is NULL
 */
struct tree_node {
	void *value;
	struct list *children;
	struct tree_node *parent;
};

/**
 * Make a new node with no children
 */
struct tree_node *tree_node_new(void *value);
/**
 * Delete the node and all of its children recursively, removing itself
 * from its parent if applicable
 */
void tree_node_del(struct tree_node *self, void (*dtor)(void *value));

struct tree_node *tree_node_prepend(struct tree_node *self, void *value);
struct tree_node *tree_node_append(struct tree_node *self, void *value);

/**
 * Given a parent and a child, move all of the child's children to parent
 * and delete child. Return the parent
 */
struct tree_node *tree_node_remove(struct tree_node *self, void (*dtor)(void *value));

/**
 * Given a parent and a root, make the root a child of parent.
 * Return a pointer to \a root
 */
struct tree_node *tree_node_insert(struct tree_node *parent, struct tree_node *root);

/**
 * Find a node given a query, NULL if not found
 */
struct tree_node *tree_node_find(struct tree_node *self, void *query, int (*cmp)(void *value, void *query));

/**
 * Map a function over all values, depth first.
 * 
 * \note
 *   If \a value is NULL, then \a func will not be called on it
 */
void tree_node_map(struct tree_node *root, void (*func)(void *value, size_t depth));

#endif // JLIB_TREE_H
