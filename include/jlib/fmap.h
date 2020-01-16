#ifndef JLIB_FMAP_H
#define JLIB_FMAP_H

#include <stddef.h> /* size_t */
#include <string.h>

#define HASH_FNV1A_PRIME (0x01000193) /* 16777619   */
#define HASH_FNV1A_SEED  (0x811C9DC5) /* 2166136261 */
size_t hash_fnv1a(const char *cstr, size_t bias, size_t max);

#define FMAP_DEFAULT_CAP 8
#define FMAP_DEFAULT_SCALING 2
#define FMAP_COLLISION_BIAS 10
#define FMAP_FULL_RATIO (2.0 / 3.0)

/* flat map or dict of string to value */
struct fmap {
	void *vals;   /* read-only */
	char **keys;  /* read-only */
	size_t size;  /* read-only, number of items contained */
	size_t cap;   /* read-only, max number of items that can be stored */
	size_t isize; /* read-only, size of a single item */
};

struct fmap *fmap_new(size_t isize);
void fmap_free(struct fmap *self);
size_t fmap_biased_index(struct fmap* self, const char *key);
int fmap_check(struct fmap *self, const char *key); /* 1 if the key is recorded, else 0 */
void fmap_grow(struct fmap *self, size_t cap);
void fmap_remove(struct fmap *self, const char *key);

#define fmap_write(Mapptr, Keystr, Value, Type) \
	do { \
		if ((Mapptr)->size > (Mapptr)->cap * FMAP_FULL_RATIO) { \
			(Mapptr)->cap *= FMAP_DEFAULT_SCALING; \
			fmap_grow((Mapptr), (Mapptr)->cap); \
		} \
		size_t index = fmap_biased_index((Mapptr), (Keystr)); \
		((Type *)((Mapptr)->vals))[index] = (Value); \
		(Mapptr)->keys[index] = calloc(strlen(Keystr) + 1, sizeof(char)); \
		strcpy((Mapptr)->keys[index], (Keystr)); \
		(Mapptr)->size++; \
	} while (0)

#define fmap_read(Mapptr, Keystr, Type) (((Type *)(Mapptr)->vals)[fmap_biased_index((Mapptr), (Keystr))])

#endif /* JLIB_FMAP_H */