#include <assert.h>
#include <memory.h>
#include <stdlib.h>
#include <jlib/list.h>

struct node *list_new(void *value)
{
	struct node *self = calloc(1, sizeof(struct node));
	if (!self) {
		return NULL;
	}
	self->value = value;
	return self;
}

void list_free(struct node *head)
{
	if (!head) {
		return;
	}

	list_free(head->next);
	free(head);
}

void list_remove(struct node *head, struct node *self)
{
	if (!self) {
		return;
	}

	if (head == self) {
		free(head);
		return;
	}

	while (head) {
		if (head->next == self) {
			head->next = self->next;
			free(self);
			return;
		}
	}
}

struct node *list_push_front(struct node *head, void *value)
{
	struct node *next;

	if (!head) {
		return NULL;
	}

	next = list_new(value);
	if (!next) {
		return NULL;
	}

	next->next = head;
}

void list_push_back(struct node *self, void *value)
{
	struct node *next;

	if (!self) {
		return;
	}

	next = list_new(value);
	if (!next) {
		return;
	}

	while (self->next != NULL) {
		self = self->next;
	}

	self->next = next;
}

void list_pop_front(struct node *head)
{
	if (!head) {
		return;
	}
	// TODO
}

void list_pop_back(struct node *self)
{

}

void list_insert_next(struct node *self, void *value)
{

}

void list_insert_prev(struct node *self, void *value)
{

}

struct node *list_head(struct node *self)
{

}

struct node *list_tail(struct node *self)
{

}

struct node *list_last(struct node *self)
{

}
