#ifndef JLIB_ARRAY_H
#define JLIB_ARRAY_H

#include <stddef.h> /* size_t */

#define ARRAY_DEFAULT_CAP 8
#define ARRAY_DEFAULT_SCALING 2

struct parray {
	void **buf;
	size_t size; /* read-only */
	size_t cap;  /* read-only */
	void (* free)(void *buf);
};

/* allocate an array on the heap, pass a function to free each item on array_free, NULL if not needed */
struct parray *parray_new(void (* free_fn)(void *buf));
void parray_free(struct parray *self);
void parray_push_(struct parray *self, void *value);
void parray_pop(struct parray *self);
void parray_resize(struct parray *self, size_t size);

#define parray_push(Arrayptr, Valueptr) parray_push_((Arrayptr), ((void*)&(Valueptr)))
#define parray_read(Arrayptr, Index, Type) (val((Arrayptr)->buf[(Index)], Type))
#define parray_write(Arrayptr, Index, Valueptr) (Arrayptr)->buf[(Index)] = ((void*)&(Valueptr))

#endif /* JLIB_ARRAY_H */