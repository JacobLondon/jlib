#include <stdlib.h>
#include <stdio.h>

#include <jlib/farray.h>

struct farray *farray_new(size_t isize)
{
	struct farray *self = malloc(sizeof(struct farray));
	if (!self) {
		fputs("Error: Could not malloc for farray init", stderr);
		exit(1);
	}

	self->buf = malloc(isize * FARRAY_DEFAULT_CAP);
	if (!self->buf) {
		fputs("Error: Could not malloc farray buffer", stderr);
		exit(1);
	}

	self->size = 0;
	self->isize = isize;
	self->cap = FARRAY_DEFAULT_CAP;

	return self;
}

void farray_free(struct farray *self)
{
	if (!self)
		return;

	/* clear each byte and free buffer */
	if (self->buf) {
		size_t i;
		for (i = 0; i < self->size * self->isize; i++)
			((char *)self->buf)[i] = 0;
		free(self->buf);
	}

	free(self);
}

void farray_pop(struct farray *self)
{
	if (self->size > 0)
		self->size--;

	/* clear the bytes of the popped item */
	size_t i;
	for (i = 0; i < self->isize; i++)
		((char *)self->buf)[self->size * self->isize + i] = 0;
}

void farray_resize(struct farray *self, size_t cap)
{
	void *tmp = realloc(self->buf, cap);
	if (!tmp) {
		fputs("Error: Could not realloc farray buffer", stderr);
		exit(1);
	}

	self->buf = tmp;
	self->cap = cap;
	if (self->size > cap)
		self->size = cap;
}
