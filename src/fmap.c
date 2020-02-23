#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include <jlib/fmap.h>

#define HASH_FNV1A_PRIME (0x01000193) /* 16777619   */
#define HASH_FNV1A_SEED  (0x811C9DC5) /* 2166136261 */

static size_t hash_fnv1a(const char *buf, size_t bias, size_t max)
{
	size_t hash = HASH_FNV1A_SEED + bias;
	while (*buf) {
		hash = (*buf++ ^ hash) * HASH_FNV1A_PRIME;
	}
	return hash % max;
}

struct fmap *fmap_new_rsrv(size_t item_size, size_t cap)
{
	struct fmap *self = malloc(sizeof(struct fmap));
	if (!self) {
		return NULL;
	}

	self->vals = calloc(cap, item_size);
	self->keys = calloc(cap, sizeof(char *));
	self->cap = FMAP_DEFAULT_CAP;
	self->item_size = item_size;

	return self;
}

void fmap_free(struct fmap *self)
{
	assert(self != NULL);

	/* clear each byte and free */
	if (self->vals) {
		size_t i;
		for (i = 0; i < self->cap * self->item_size; i++)
			((char *)self->vals)[i] = 0;
		free(self->vals);
	}
	/* free each key, clear, then free the array */
	if (self->keys) {
		size_t i;
		for (i = 0; i < self->cap; i++) {
			if (self->keys[i]) {
				free(self->keys[i]);
				self->keys[i] = NULL;
			}
		}
		free(self->keys);
	}
	self->cap = 0;
	self->item_size = 0;
	self->keys = NULL;
	self->size = 0;
	self->vals = NULL;
	free(self);
}

size_t fmap_biased_index(struct fmap* self, const char *key)
{
	size_t index;
	size_t bias = 0;
	do {
		index = hash_fnv1a(key, bias, self->cap);
		bias += FMAP_COLLISION_BIAS;
	/* valid key and keys not equal */
	} while (self->keys[index] != NULL && strcmp(self->keys[index], key) != 0);
	return index;
}

int fmap_check(struct fmap *self, const char *key)
{
	size_t index = fmap_biased_index(self, key);
	return self->keys[index] ? 1 : 0;
}

int fmap_grow_by(struct fmap *self, int mod)
{
	assert(self != NULL);
	assert(mod >= 1);

	/* swap data */
	void  *oldvals = self->vals;
	char **oldkeys = self->keys;
	const size_t oldcap = self->cap;
	self->cap *= mod;

	self->vals = calloc(self->cap, self->item_size);
	if (!self->vals) {
		self->vals = oldvals;
		return 0;
	}
	self->keys = calloc(self->cap, sizeof(char *));
	if (!self->keys) {
		free(self->vals);
		self->vals = oldvals;
		self->keys = oldkeys;
		return 0;
	}

	size_t i, j, index;
	for (i = 0; i < oldcap; i++) {
		if (oldkeys[i]) {
			index = fmap_biased_index(self, oldkeys[i]);
			/* copy value bytes */
			for (j = 0; j < self->item_size; j++) {
				((char *)self->vals)[index * self->item_size + j] = ((char *)oldvals)[i * self->item_size + j];
				((char *)oldvals)[i * self->item_size + j] = 0;
			}
			
			/* move key */
			self->keys[index] = oldkeys[i];
			oldkeys[i] = NULL;
		}
	}
	free(oldvals);
	free(oldkeys);

	return 1;
}

void fmap_remove(struct fmap *self, const char *key)
{
	assert(self != NULL);
	assert(key != NULL);

	size_t index = fmap_biased_index(self, key);
	size_t i;

	for (i = 0; i < self->item_size; i++) {
		((char *)self->vals)[index * self->item_size + i] = 0;
	}

	free(self->keys[index]);
	self->keys[index] = NULL;
}
