#ifndef JLIB_GC_H
#define JLIB_GC_H

#include <stddef.h>
#include <malloc.h>

struct ref {
	void *val;                /* read-only, owner */
	void (*dtor)(void *self); /* read-only */
};

struct gc {
	struct ref *refs; /* read-only */
	size_t size;      /* read-only */
	size_t cap;       /* read-only */
};

/**
 * Create a new garbage collector on the heap
 */
struct gc *gc_new(void);
/**
 * Free a garbage collector and collect ALL of the garbage
 */
void gc_free(struct gc *self);

/**
 * Free a pointer by value, need to look for it though
 */
void gc_free_ref(struct gc *self, void *buf);

/**
 * 'calloc' memory and the GC owns it, the pointer is returned,
 * and the desctructor (nullable) is specified.
 */
void *gc_alloc_dtor(struct gc *self, size_t size, void (*dtor)(void *val));
/**
 * 'calloc' memory and the GC owns it, the pointer is returned,
 * and the destructor automatically 'free'
 */
#define gc_alloc(self, size) gc_alloc_dtor(self, size, free)

/**
 * Transfer ownership of an already allocated val to the GC
 */
void gc_push_dtor(struct gc *self, void *val, void (*dtor)(void *val));

/**
 * Transfer ownership, specify free as the dtor
 */
#define gc_push(self, val) gc_push_dtor(self, val, free)

/**
 * Mark a point to collect to
 */
void gc_mark(struct gc *self);
/**
 * Collect all garbage until the most recent mark,
 * or until there is no more garbage
 */
void gc_collect(struct gc *self);

/**
 * Dump detailed info about all allocations
 */
void gc_print(struct gc *self);

#endif /* JLIB_GC_H */