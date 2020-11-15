#include <assert.h>
#include <memory.h>

#include <jlib/tree.h>

static int tree_node_ptr_cmp(void *lhs, void *rhs)
{
	return (lhs == rhs) ? 0 : 1;
}

struct tree_node *tree_node_new(void *value)
{
	struct tree_node *self;
	self = malloc(sizeof(*self));
	if (!self) {
		return NULL;
	}

	(void)memset(self, 0, sizeof(*self));
	self->value = value;

	return self;
}

void tree_node_del(struct tree_node *self, void (*dtor)(void *value))
{
	struct list_node **cursor;
	assert(self);

	if (dtor && self->value) {
		dtor(self->value);
		self->value = NULL;
	}

	// is a leaf node
	if (!self->children) {
		return;
	}

	// has children
	for (cursor = list_iter_begin(self->children);
	     !list_iter_done(cursor);
	     list_iter_continue(&cursor))
	{
		tree_node_del(list_iter_value(cursor), dtor);
	}
	list_free(self->children);
	free(self);
}

void tree_node_prepend(struct tree_node *self, void *value)
{
	struct tree_node *node;
	assert(self);
	
	node = tree_node_new(value);
	assert(node);

	// turn leaf node into a parent
	if (self->children == NULL) {
		self->children = list_new(NULL); // manually free the items...
		assert(self->children);
	}

	list_push_front(self->children, node);
}

void tree_node_append(struct tree_node *self, void *value)
{
	struct tree_node *node;
	assert(self);
	
	node = tree_node_new(value);
	assert(node);

	// turn leaf node into a parent
	if (self->children == NULL) {
		self->children = list_new(NULL); // manually free the items...
		assert(self->children);
	}

	list_push_back(self->children, node);
}

struct tree_node *tree_node_remove(struct tree_node *self, void (*dtor)(void *value))
{
	struct tree_node *parent;
	struct list_node *cursor;
	assert(self);
	parent = self->parent;

	if (self->value && dtor) {
		dtor(self->value);
	}

	// self has children who will need a new parent
	if (self->children) {
		// there is a parent to take the children
		if (parent) {
			// remove self from parent's children
			cursor = list_find(parent->children, self, tree_node_ptr_cmp);
			list_remove(parent->children, cursor);
			// give children's children to parent
			list_take(parent->children, self->children);
		}
		// there is no parent, so make left-most child the new root
		else {
			cursor = list_head(self->children);
			parent = cursor->value;
			list_remove(self->children, cursor);
			// new parent already has some children
			if (parent->children) {
				list_take(parent->children, self->children);
			}
			// new parent has no children
			else {
				parent->children = list_new(NULL);
				assert(parent->children);
				list_take(parent->children, self->children);
			}
		}
		list_free(self->children);
	}

	free(self);
	return parent;
}

struct tree_node *tree_node_find(struct tree_node *self, void *query, int (*cmp)(void *value, void *query))
{
	assert(self);
	assert(query);
	assert(cmp);

	if (self->value) {
		if (cmp(self->value, query) == 0) {
			return self;
		}
	}

	if (!self->children) {
		return NULL;
	}

	struct list_node **cursor;
	struct tree_node *node;
	for (cursor = list_iter_begin(self->children);
	     !list_iter_done(cursor);
	     list_iter_continue(&cursor))
	{
		node = tree_node_find(list_iter_value(cursor), query, cmp);
		if (node != NULL) {
			return node;
		}
	}
	return NULL;
}
