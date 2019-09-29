#ifndef JLIB_ALLOCATOR_H
#define JLIB_ALLOCATOR_H

#define allocate(object) object->alloc->new(object)
#define delete(object) object->alloc->del(object)

typedef struct allocator_tag {
    int (* new)(void *buf);
    int (* del)(void *buf);
} allocator;

allocator *allocator_init(int (* new)(void *buf), int (*del)(void *buf));
void allocator_free(allocator *self);

#endif // JLIB_ALLOCATOR_H