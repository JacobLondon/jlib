#ifndef JLIB_ARRAY_H
#define JLIB_ARRAY_H

#include <stddef.h> /* size_t */

#define PARRAY_DEFAULT_CAP 8
#define PARRAY_DEFAULT_SCALING 2

/* pointer array */
struct parray {
	void **buf;
	size_t size; /* read-only */
	size_t cap;  /* read-only */
	void (* dtor)(void *buf);
};

/**
 * Allocate an array on the heap, pass a function
 * to free each item on array_free, NULL if not needed
 */
struct parray *parray_new(void (* dtor)(void *buf));

/**
 * Free a parray and all of its members
 */
void parray_free(struct parray *self);

/**
 * Write a value at the end of the parray
 */
void parray_push_(struct parray *self, void *value);

/**
 * Write a value at the end of the array, automatically convert LValue to void*
 */
#define parray_push(Arrayptr, LValue) parray_push_((Arrayptr), ((void*)&(LValue)))

/**
 * Remove and free the last item in the parray
 */
void parray_pop(struct parray *self);

/**
 * Change the maximum capacity of an farray
 */
int parray_resize(struct parray *self, size_t cap);

/**
 * RValue of item at the Index of the farray, and dereference it
 */
#define parray_read(Arrayptr, Index, Type) (*(Type *)((Arrayptr)->buf[(Index)]))

/**
 * Write the LValue at the parray's Index
 */
#define parray_write(Arrayptr, Index, LValue) \
	do { \
		(Arrayptr)->buf[(Index)] = ((void*)&(LValue)); \
	} while (0)

#endif /* JLIB_ARRAY_H */
