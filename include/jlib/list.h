#ifndef JLIB_LIST_H
#define JLIB_LIST_H

#include <stddef.h> /* size_t */
#include <malloc.h>

struct node {
	void *value;
	struct node *next;
};

/**
 * Create a new, empty list
 * 
 * dtor: Specity a way to free the value
 */
struct node *list_new();

/**
 * Free every value in a list, every node, and the list itself
 */
void list_free(struct node *head);

/**
 * Free a value and its node, rejoin adjacent nodes
 */
void list_remove(struct node *head, struct node *self);

/**
 * Make a new node at the head location. Return the new head
 */
struct node *list_push_front(struct node *head, void *value);

/**
 * Make a new node at the tail location
 */
void list_push_back(struct node *self, void *value);

/**
 * Free the first node and its value, repoint the head node
 */
void list_pop_front(struct node *head);

/**
 * Free the last node and its value, repoint the tail node
 */
void list_pop_back(struct node *self);

/**
 * Make a new node after the current node pointer in the list
 */
void list_insert_next(struct node *self, void *value);

/**
 * Make a new node before the current node pointer in the list
 */
void list_insert_prev(struct node *self, void *value);

/**
 * First element
 */
struct node *list_head(struct node *self);

/**
 * Second element
 */
struct node *list_tail(struct node *self);

/**
 * Last element
 */
struct node *list_last(struct node *self);

#endif /* JLIB_LIST_H */
