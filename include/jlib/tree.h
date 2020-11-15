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

void tree_node_prepend(struct tree_node *self, void *value);
void tree_node_append(struct tree_node *self, void *value);

/**
 * Given a parent and a child, move all of the child's children to parent
 * and delete child. Return the parent
 */
struct tree_node *tree_node_remove(struct tree_node *self, void (*dtor)(void *value));

/**
 * Find a node given a query, NULL if not found
 */
struct tree_node *tree_node_find(struct tree_node *self, void *query, int (*cmp)(void *value, void *query));

/**
 * Swap self with parent
 */
void tree_node_swap(struct tree_node *self);

/**
 * Given a \a parent collect \a number of its children into a new tree_node
 * and make that tree_node the child of \a parent.
 * 
 * \example
 *     a
 *   / | \
 *  b  c  d
 * 
 * tree_node_collect(a, 2)
 * 
 *      a
 *    /   \
 *   e     d
 *  | \
 *  b  c
 * 
 * \param parent
 *   The node to operate on
 * \param number
 *   The number of items to collect.
 *   0 to collect ALL items
 * \return
 *   0  success
 *   1  \a number is larger than there are nodes to collect
 *   2  parent has no children to collect
 */
int tree_node_collect(struct tree_node *parent, int number);

#endif // JLIB_TREE_H
