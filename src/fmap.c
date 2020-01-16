#include <stdlib.h>
#include <stdio.h>

#include <jlib/fmap.h>

size_t hash_fnv1a(const char *cstr, size_t bias, size_t max)
{
	size_t hash = HASH_FNV1A_SEED + bias;
	while (*cstr)
		hash = (*cstr++ ^ hash) * HASH_FNV1A_PRIME;
	return hash % max;
}

struct fmap *fmap_new(size_t isize)
{
	struct fmap *self = malloc(sizeof(struct fmap));
	if (!self) {
		fputs("Error: Could not malloc fmap init", stderr);
		exit(-1);
	}

	self->vals = calloc(FMAP_DEFAULT_CAP, isize);
	self->keys = calloc(FMAP_DEFAULT_CAP, sizeof(char *));
	self->cap = FMAP_DEFAULT_CAP;
	self->isize = isize;

	return self;
}

void fmap_free(struct fmap *self)
{
	if (!self)
		return;

	/* clear each byte and free */
	if (self->vals) {
		size_t i;
		for (i = 0; i < self->cap * self->isize; i++)
			((char *)self->vals)[i] = 0;
		free(self->vals);
	}
	/* free each key, clear, then free the array */
	if (self->keys) {
		size_t i;
		for (i = 0; i < self->cap; i++) {
			if (self->keys[i])
				free(self->keys[i]);
			self->keys[i] = NULL;
		}
		free(self->keys);
	}

	free(self);
}

/* return first unused index found */
size_t fmap_biased_index(struct fmap* self, const char *key)
{
	size_t index;
	size_t bias = 0;
	do {
		index = hash_fnv1a(key, bias, self->cap);
		bias += FMAP_COLLISION_BIAS;
	/* valid key and keys not equal */
	} while (self->keys[index] && strcmp(self->keys[index], key) != 0);
	return index;
}

int fmap_check(struct fmap *self, const char *key)
{
	size_t index = fmap_biased_index(self, key);
	return self->keys[index] ? 1 : 0;
}

void fmap_grow(struct fmap *self, size_t cap)
{
	if (self->cap > cap) {
		fputs("Error: Tried to grow fmap to a smaller capacity", stderr);
		exit(-1);
	}
	
	/* swap data */
	void  *oldvals = self->vals;
	char **oldkeys = self->keys;
	size_t oldcap  = self->cap;
	self->vals = calloc(cap, self->isize);
	if (!self->vals) {
		fputs("Error: Could not calloc values in fmap resize", stderr);
		exit(-1);
	}
	self->keys = calloc(cap, sizeof(char *));
	if (!self->keys) {
		fputs("Error: Could not calloc keys in fmap resize", stderr);
		exit(-1);
	}
	self->cap  = cap;

	size_t i, j, index;
	for (i = 0; i < oldcap; i++) {
		if (oldkeys[i]) {
			index = fmap_biased_index(self, oldkeys[i]);
			/* copy value bytes */
			for (j = 0; j < self->isize; j++)
				((char *)self->vals)[index * self->isize + j] = ((char *)oldvals)[i * self->isize + j];
			
			/* move key */
			self->keys[index] = oldkeys[i];
		}
	}
	free(oldvals);
}

void fmap_remove(struct fmap *self, const char *key)
{
	size_t index = fmap_biased_index(self, key);
	size_t i;
	for (i = 0; i < self->isize; i++)
		((char *)self->vals)[index * self->isize + i] = 0;

	free(self->keys[index]);
	self->keys[index] = NULL;
}
