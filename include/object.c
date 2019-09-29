#include <stdlib.h> // malloc, free
#include "object.h"

object *object_init()
{
    object *self = malloc(sizeof(object));
    self->alloc = allocator_init(object_new, object_del);
    allocate(self);
    return self;
}

int object_new(void *buf)
{
    object *self = (object *)(buf);
    self->buf = NULL;
    return 1;
}

int object_del(void *buf)
{
    if (!buf)
        return 1;
    
    object *self = (object *)(buf);
    if (self->buf)
        free(self->buf);
    if (self->alloc)
        allocator_free(self->alloc);
    free(buf);
    
    return 1;
}
