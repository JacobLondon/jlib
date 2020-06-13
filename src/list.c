#include <assert.h>
#include <memory.h>
#include <stdlib.h>
#include <jlib/list.h>

struct list *list_new(void)
{
	struct list *self = calloc(1, sizeof(struct list));
	assert(self);
	self->head = self->tail;
	return self;
}

void list_free(struct list *self)
{
	assert(self);

	/* free each node */
	while (self->head) {
		list_remove(self, self->head);
	}

skip:
	memset(self, 0, sizeof(struct list));
	free(self);
}

void list_remove(struct list *self, struct node *cursor)
{
	assert(self);

	/* nothing to do */
	if (!cursor) {
		return;
	}

	if (cursor->next) {
		cursor->next->prev = cursor->prev;
	}
	// is tail
	else {
		self->tail = cursor->prev;
	}

	if (cursor->prev) {
		cursor->prev->next = cursor->next;
	}
	// is head
	else {
		self->head = cursor->next;
	}

	if (cursor->value) {
		free(cursor->value);
	}

	memset(cursor, 0, sizeof(struct node));
	free(cursor);	
}

struct node *list_push_front(struct list *self, void *value)
{
	return list_insert_prev(self, self->head, value);
}

struct node *list_push_back(struct list *self, void *value)
{
	return list_insert_next(self, self->tail, value);
}

void list_pop_front(struct list *self)
{
	list_remove(self, self->head);
}

void list_pop_back(struct list *self)
{
	list_remove(self, self->tail);
}

struct node *list_insert_next(struct list *self, struct node *cursor, void *value)
{
	assert(self);

	struct node *new = malloc(sizeof(struct node));
	assert(new);
	new->value = value;

	if (cursor) {
		new->next = cursor->next;
		new->prev = cursor;
		cursor->next = new;

		if (cursor->next) {
			cursor->next->prev = new;
		}
		// is tail
		else {
			self->tail = new;
		}
	}
	// inserting after at tail
	else {
		if (self->tail) {
			new->prev = self->tail->prev;
			new->prev->next = new;
		}
		else {
			new->prev = NULL;
		}
		new->next = NULL;

		self->tail = new;
	}

	return new;
}

struct node *list_insert_prev(struct list *self, struct node *cursor, void *value)
{
	assert(self);

	struct node *new = malloc(sizeof(struct node));
	assert(new);
	new->value = value;

	if (cursor) {
		new->next = cursor;
		new->prev = cursor->prev;
		cursor->prev = new;

		if (cursor->prev) {
			cursor->prev->next = new;
		}
		// is head
		else {
			self->head = new;
		}
	}
	// inserting before at head
	else {
		if (self->head) {
			new->next = self->head->next;
			new->next->prev = new;
		}
		else {
			new->next = NULL;
		}
		new->prev = NULL;

		self->head = new;
	}

	return new;
}

void list_remove_next(struct list *self, struct node *cursor)
{
	list_remove(self, cursor->next);
}

void list_remove_prev(struct list *self, struct node *cursor)
{
	list_remove(self, cursor->prev);
}
