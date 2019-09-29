#include <stdlib.h>
#include "allocator.h"

allocator *allocator_init(int (* new)(void *buf), int (*del)(void *buf))
{
    allocator *self = malloc(sizeof(allocator));
    self->new = new;
    self->del = del;
    return self;
}

void allocator_free(allocator *self)
{
    if (!self)
        return;

    self->new = NULL;
    self->del = NULL;
    free(self);
    self = NULL;
}
