#include <stdlib.h> // malloc, free
#include "owned.h"

owned *owned_init()
{
    owned *self = malloc(sizeof(owned));
    self->alloc = allocator_init(owned_new, owned_del);
    allocate(self);
    return self;
}

int owned_new(void *owner)
{
    owned *self = (owned *)owner;
    self->buf = NULL;
    return 1;
}

int owned_del(void *owner)
{
    if (!owner)
        return 1;
    
    owned *self = (owned *)owner;
    if (self->buf) {
        free(self->buf);
        self->buf = NULL;
    }
    if (self->alloc) {
        allocator_free(self->alloc);
        self->alloc = NULL;
    }
    free(owner);

    return 1;
}
