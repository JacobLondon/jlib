#ifndef JLIB_ALLOCATOR_H
#define JLIB_ALLOCATOR_H

#define allocate(object) object->alloc->new(object)
#define delete(object) object->alloc->del(object)
#define lifetime(object) \
    for (unsigned char jlib_lifetime_##object = 1; jlib_lifetime_##object != 0; delete(object), jlib_lifetime_##object--)

typedef struct allocator_tag {
    int (* new)(void *owner);
    int (* del)(void *owner);
} allocator;

allocator *allocator_init(int (* new)(void *owner), int (*del)(void *owner));
void allocator_free(allocator *self);

void allocator_test();

#endif // JLIB_ALLOCATOR_H