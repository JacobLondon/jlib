#include <stdlib.h>
#include <stdio.h>

#include <jlib/array.h>

struct array_s *array_new(void (* f_free)(void *buf))
{
	struct array_s *self = malloc(sizeof(struct array_s));
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
	self->free = f_free;

	return self;
}

void array_free(struct array_s *self)
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

void array_push_(struct array_s *self, void *value)
{
	/* realloc for more space */
	if (self->size == self->cap) {
		self->cap = self->cap * ARRAY_DEFAULT_SCALING;
		array_resize(self, self->cap);
	}

	*(self->buf + self->size++) = value;
}

void array_pop(struct array_s *self)
{
	if (self->size > 0)
		self->size--;

	if (self->free && self->buf[self->size])
		self->free(self->buf[self->size]);

	self->buf[self->size] = NULL;
}

void array_resize(struct array_s *self, size_t size)
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
