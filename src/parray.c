#include <stdlib.h>
#include <stdio.h>

#include <jlib/array.h>

struct parray *parray_new(void (* free_fn)(void *buf))
{
	struct parray *self = malloc(sizeof(struct parray));
	if (!self) {
		fputs("Error: Could not malloc for array init", stderr);
		exit(-1);
	}

	self->buf = malloc(sizeof(void *) * ARRAY_DEFAULT_CAP);
	if (!self->buf) {
		fputs("Error: Could not malloc array buffer", stderr);
		exit(-1);
	}

	self->size = 0;
	self->cap  = ARRAY_DEFAULT_CAP;
	self->free = free_fn;

	return self;
}

void parray_free(struct parray *self)
{
	if (!self)
		return;

	if (!(self->buf && self->free))
		goto Skip;

	size_t i;
	for (i = 0; i < self->size; i++) {
		if (*(self->buf + i)) {
			self->free(*(self->buf + i));
		}
	}

Skip:
	free(self);
}

void parray_push_(struct parray *self, void *value)
{
	/* realloc for more space */
	if (self->size == self->cap) {
		self->cap = self->cap * ARRAY_DEFAULT_SCALING;
		array_resize(self, self->cap);
	}

	*(self->buf + self->size++) = value;
}

void parray_pop(struct parray *self)
{
	if (self->size > 0)
		self->size--;

	if (self->free && self->buf[self->size])
		self->free(self->buf[self->size]);

	self->buf[self->size] = NULL;
}

void parray_resize(struct parray *self, size_t size)
{
	void **tmp = realloc(self->buf, size);
	if (!tmp) {
		fputs("Error: Could not realloc array buffer", stderr);
		exit(-1);
	}

	self->buf = tmp;
	self->cap = size;
	if (self->size > size)
		self->size = size;
}
