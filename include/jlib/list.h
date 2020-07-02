#ifndef JLIB_LIST_H
#define JLIB_LIST_H

#include <stddef.h> /* size_t */
#include <malloc.h>

struct node {
	void *value;
	struct node *next;
	struct node *prev;
};

struct list {
	struct node *head;
	struct node *tail;
	size_t size;
	void (*dtor)(void *buf);
};

/**
 * Create a new, empty list
 */
struct list *list_new_dtor(void (*dtor)(void *buf));
#define list_new() list_new_dtor(free)

/**
 * Free every value in a list, every node, and the list itself
 */
void list_free(struct list *self);

/**
 * Free a value and its node, rejoin adjacent nodes
 */
void list_remove(struct list *self, struct node *cursor);

/**
 * Make a new node at the head location
 */
struct node *list_push_front(struct list *self, void *value);

/**
 * Make a new node at the tail location
 */
struct node *list_push_back(struct list *self, void *value);

/**
 * Free the first node and its value, repoint the head node
 */
void list_pop_front(struct list *self);

/**
 * Free the last node and its value, repoint the tail node
 */
void list_pop_back(struct list *self);

/**
 * Make a new node after the current node pointer in the list
 */
struct node *list_insert_next(struct list *self, struct node *cursor, void *value);

/**
 * Make a new node before the current node pointer in the list
 */
struct node *list_insert_prev(struct list *self, struct node *cursor, void *value);

/**
 * Free the next node and its value, rejoin the adjacent nodes
 */
void list_remove_next(struct list *self, struct node *cursor);

/**
 * Free the prev node and its value, rejoin the adjacent nodes
 */
void list_remove_prev(struct list *self, struct node *cursor);

#endif /* JLIB_LIST_H */
