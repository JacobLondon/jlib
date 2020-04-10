#ifndef JLIB_FARRAY_H
#define JLIB_FARRAY_H

#include <stddef.h> /* size_t */

#define FARRAY_DEFAULT_CAP 8
#define FARRAY_DEFAULT_SCALING 2

/* flat array */
struct farray {
	void *buf;
	size_t size;      /* read-only */
	size_t cap;       /* read-only */
	size_t item_size; /* read-only, size of a single item */
};

/**
 * Create a new farray on the heap with a reserved capacity
 */
struct farray *farray_new_rsrv(size_t item_size, size_t cap);

/**
 * Create a new fmap on the heap with default capacity
 */
#define farray_new(item_size) farray_new_rsrv(item_size, FARRAY_DEFAULT_CAP)

/**
 * Free an farray and all of its members
 */
void farray_free(struct farray *self);

/**
 * Clear the last item from an farray
 */
void farray_pop(struct farray *self);

/**
 * Change the maximum capacity of an farray
 */
void farray_resize(struct farray *self, size_t cap);

/**
 * Write the Value at the end of the farray
 */
#define farray_push(Arrayptr, Value, Type) \
	do { \
		if ((Arrayptr)->size == (Arrayptr)->cap) { \
			farray_resize((Arrayptr), (Arrayptr)->cap * FARRAY_DEFAULT_SCALING); \
		} \
		((Type *)((Arrayptr)->buf))[(Arrayptr)->size++] = (Value); \
	} while (0)

/**
 * RValue of item at the Index of the farray
 */
#define farray_read(Arrayptr, Index, Type) (((Type *)((Arrayptr)->buf))[(Index)])

/**
 * Write the Value at the farray's Index
 */
#define farray_write(Arrayptr, Index, Value, Type) \
	do { \
		((Type *)((Arrayptr)->buf))[(Index)] = (Value); \
	} while (0)

#endif /* JLIB_FARRAY_H */