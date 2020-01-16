#ifndef JLIB_FARRAY_H
#define JLIB_FARRAY_H

#include <stddef.h> /* size_t */

#define FARRAY_DEFAULT_CAP 8
#define FARRAY_DEFAULT_SCALING 2

/* flat array */
struct farray {
	void *buf;
	size_t size; /* read-only */
	size_t cap;  /* read-only */
	size_t isize; /* read-only, size of a single item */
};

/* allocate an array on the heap, pass a function to free each item on array_free, NULL if not needed */
struct farray *farray_new(size_t isize, void (* free_fn)(void *buf));
void farray_free(struct parray *self);
void farray_pop(struct parray *self);
void farray_resize(struct parray *self, size_t cap);

#define farray_push(Arrayptr, Value, Type) \
	do { \
		if ((Arrayptr)->size == (Arrayptr)->cap) { \
			(Arrayptr)->cap = (Arrayptr)->cap * FARRAY_DEFAULT_SCALING; \
			farray_resize((Arrayptr), (Arrayptr)->cap); \
		} \
		((Type *)((Arrayptr)->buf))[(Arrayptr)->size++] = (Value); \
	} while (0)

#define farray_read(Arrayptr, Index, Type) (((Type *)((Arrayptr)->buf))[(Index)])

#define farray_write(Arrayptr, Index, Type, Value) \
	do { \
		((Type *)((Arrayptr)->buf))[(Index)] = (Value); \
	} while (0)

#endif /* JLIB_FARRAY_H */