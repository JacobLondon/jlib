#include <assert.h>
#include <stdio.h>
#include <jlib/gc.h>

#define GC_DEFAULT_CAP 8
#define GC_DEFAULT_SCALING 2

static void ref_free(struct ref *self);

static void ref_free(struct ref *self)
{
	assert(self);
	if (self->dtor) {
		assert(self->val);
		self->dtor(self->val);
		self->dtor = NULL;
	}
	self->val = NULL;
}

static void gc_resize(struct gc *self, size_t cap)
{
	assert(self);
	void *tmp = realloc(self->refs, sizeof(struct ref) * cap);
	assert(tmp);
	self->refs = (struct ref *)tmp;
	self->cap = cap;
	if (self->size > cap) {
		self->size = cap;
	}
}

struct gc *gc_new(void)
{
	struct gc *self = malloc(sizeof(struct gc));
	assert(self);
	self->refs = calloc(GC_DEFAULT_CAP, sizeof(struct ref));
	assert(self->refs);

	self->size = 0;
	self->cap = GC_DEFAULT_CAP;

	return self;
}

void gc_free(struct gc *self)
{
	assert(self);
	assert(self->refs);
	
	while (self->size) {
		gc_collect(self);
	}
	free(self->refs);
	self->refs = NULL;
	self->cap = 0;
	self->size = 0;
	free(self);
}

void *gc_alloc_dtor(struct gc *self, size_t size, void (*dtor)(void *val))
{
	assert(self);
	void *val = calloc(1, size);
	assert(val);
	gc_push(self, val, dtor);
	return val;
}

void gc_mark(struct gc *self)
{
	assert(self);
	gc_push(self, self, NULL);
}

void gc_push(struct gc *self, void *val, void (*dtor)(void *val))
{
	assert(self);

	if (self->size == self->cap) {
		gc_resize(self, self->cap * GC_DEFAULT_SCALING);
	}

	/* only allocate for a new item, not a marker */
	if (val != self) {
		self->refs[self->size] = (struct ref){val, dtor};
	}
	else {
		self->refs[self->size] = (struct ref){(void *)self, NULL};
	}

	self->size++;
}

void gc_collect(struct gc *self)
{
	assert(self);

	size_t i = 0;

	/* remove all items after most recent mark */
	for (i = self->size - 1;
		(i < self->cap) &&
		(self->refs[i].val != (void *)self);
		i--)
	{
		ref_free(&self->refs[i]);
		self->size--;
	}

	/* remove marker if not at beginning */
	if (self->refs[i].val == (void *)self) {
		ref_free(&self->refs[i]);
		self->size--;
	}
}
