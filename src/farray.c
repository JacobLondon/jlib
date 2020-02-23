#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include <jlib/farray.h>

struct farray *farray_new_rsrv(size_t item_size, size_t cap)
{
	assert(item_size > 0);

	struct farray *self = malloc(sizeof(struct farray));
	if (!self) {
		return NULL;
	}

	self->buf = malloc(item_size * cap);
	if (!self->buf) {
		free(self);
		return NULL;
	}

	self->size = 0;
	self->item_size = item_size;
	self->cap = cap;

	return self;
}

void farray_free(struct farray *self)
{
	assert(self != NULL);

	/* clear each byte and free buffer */
	if (self->buf) {
		size_t i;
		for (i = 0; i < self->size * self->item_size; i++)
			((char *)self->buf)[i] = 0;
		free(self->buf);
	}

	self->buf = NULL;
	self->cap = 0;
	self->item_size = 0;
	self->size = 0;
	free(self);
}

void farray_pop(struct farray *self)
{
	assert(self != NULL);

	if (self->size > 0) {
		self->size--;
	}

	/* clear the bytes of the popped item */
	size_t i;
	for (i = 0; i < self->item_size; i++) {
		((char *)self->buf)[self->size * self->item_size + i] = 0;
	}
}

int farray_resize(struct farray *self, size_t cap)
{
	assert(self != NULL);
	assert(cap > 0);

	void *tmp = realloc(self->buf, self->item_size * cap);
	if (!tmp) {
		return 0;
	}

	self->buf = tmp;
	self->cap = cap;
	if (self->size > cap) {
		self->size = cap;
	}
	return 1;
}
