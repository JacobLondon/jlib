#include <stdlib.h>

#include <jlib/parray.h>

struct parray *parray_new(void (* dtor)(void *buf))
{
	struct parray *self = malloc(sizeof(struct parray));
	if (!self) {
		return NULL;
	}

	self->buf = malloc(sizeof(void *) * PARRAY_DEFAULT_CAP);
	if (!self->buf) {
		free(self);
		return NULL;
	}

	self->size = 0;
	self->cap  = PARRAY_DEFAULT_CAP;
	self->dtor = dtor;

	return self;
}

void parray_free(struct parray *self)
{
	if (!self)
		return;

	/* clear and free buffer */
	if (self->buf) {
		size_t i;
		for (i = 0; i < self->size; i++) {
			if (*(self->buf + i) && self->dtor) {
				self->dtor(*(self->buf + i));
			}
			*(self->buf + i) = NULL;
		}
		free(self->buf);
	}

	self->buf = NULL;
	self->cap = 0;
	self->dtor = NULL;
	self->size = 0;
	free(self);
}

void parray_push(struct parray *self, void *value)
{
	/* realloc for more space */
	if (self->size == self->cap) {
		parray_resize(self, self->cap * PARRAY_DEFAULT_SCALING);
	}

	*(self->buf + self->size++) = value;
}

void parray_pop(struct parray *self)
{
	if (self->size > 0) {
		self->size--;
	}

	if (self->dtor && self->buf[self->size]) {
		self->dtor(self->buf[self->size]);
	}

	self->buf[self->size] = NULL;
}

int parray_resize(struct parray *self, size_t cap)
{
	void **tmp = realloc(self->buf, sizeof(void *) * cap);
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
