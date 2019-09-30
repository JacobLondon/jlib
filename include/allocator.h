#ifndef JLIB_ALLOCATOR_H
#define JLIB_ALLOCATOR_H

#define allocate(object) object->alloc->new(object)
#define delete(object) object->alloc->del(object)
#define lifetime(object) \
    for (unsigned char JLIB_LIFETIME_##object = 1; JLIB_LIFETIME_##object != 0; delete(object), JLIB_LIFETIME_##object--)

typedef struct allocator_tag {
    int (* new)(void *owner);
    int (* del)(void *owner);
} allocator;

allocator *allocator_init(int (* new)(void *owner), int (*del)(void *owner));
void allocator_free(allocator *self);

void allocator_test();

#endif // JLIB_ALLOCATOR_H