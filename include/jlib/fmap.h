#ifndef JLIB_FMAP_H
#define JLIB_FMAP_H

#include <stddef.h> /* size_t */
#include <string.h>

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
	size_t item_size; /* read-only, size of a single item */
};

/**
 * Create a new fmap on the heap with a reserved capacity
 */
struct fmap *fmap_new_rsrv(size_t item_size, size_t cap);

/**
 * Create a new fmap on the heap with default capacity
 */
#define fmap_new(item_size) fmap_new_rsrv(item_size, FMAP_DEFAULT_CAP)

/**
 * Free an fmap and all of its members
 */
void fmap_free(struct fmap *self);

/**
 * Return first unused index or the index of the existing key
 */
size_t fmap_biased_index(struct fmap* self, const char *key);

/**
 * Return 1 if the key is found, else 0
 */
int fmap_check(struct fmap *self, const char *key);

/**
 * Grow capacity by mod,
 * expects mod >= 1
 * 
 * Return 1 if success, else 0
 */
int fmap_grow_by(struct fmap *self, int mod);

/**
 * Grow capacity to Cap, may be slightly off based on integer division
 * 
 * Return 1 if success, else 0
 */
#define fmap_grow_to(Mapptr, Cap) fmap_grow((Mapptr), (Cap) / (Mapptr)->cap)

/**
 * Clear the item and key based on the given key
 */
void fmap_remove(struct fmap *self, const char *key);

/**
 * Write the Value at the Keystr hash's index
 */
#define fmap_write(Mapptr, Keystr, Value, Type) \
	do { \
		if ((Mapptr)->size > (Mapptr)->cap * FMAP_FULL_RATIO) { \
			if (fmap_grow_by((Mapptr), FMAP_DEFAULT_SCALING) == 0) { \
				break; \
			} \
		} \
		size_t no_collide_index_ = fmap_biased_index((Mapptr), (Keystr)); \
		((Type *)((Mapptr)->vals))[no_collide_index_] = (Value); \
		(Mapptr)->keys[no_collide_index_] = calloc(strlen(Keystr) + 1, sizeof(char)); \
		(void)strcpy((Mapptr)->keys[no_collide_index_], (Keystr)); \
		(Mapptr)->size++; \
	} while (0)

/**
 * RValue of item at the Keystr hash's index
 */
#define fmap_read(Mapptr, Keystr, Type) (((Type *)(Mapptr)->vals)[fmap_biased_index((Mapptr), (Keystr))])

/**
 * Perform a for each operation on an fmap
 * 
 * Keyiter is a char * representing the current key
 * Valueiter is a Type (NOT a pointer!) representing the current value
 * Type is the Value's type
 * Codeblock is a block of code where the described values can be used
 * 
 * struct fmap *map = fmap_new(sizeof(int));
 * char *key;
 * int value;
 * fmap_for_each(map, key, value, int, {
 *     printf("%s: %d\n", key, value);
 * });
 */
#define fmap_for_each(Mapptr, Keyiter, Valueiter, Type, Codeblock) \
	do { \
		size_t no_collide_i_; \
		for (no_collide_i_ = 0; no_collide_i_ < (Mapptr)->cap; no_collide_i_++) { \
			if ((Mapptr)->keys[no_collide_i_] != NULL) { \
				(Valueiter) = ((Type *)((Mapptr)->vals))[no_collide_i_]; \
				(Keyiter)   = (Mapptr)->keys[no_collide_i_]; \
				Codeblock \
			} \
		} \
	} while (0)

#endif /* JLIB_FMAP_H */