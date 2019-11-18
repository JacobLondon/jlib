#include <stdlib.h>

#include "array.h"

Array *array_new(void (* free_fn)(void *buf))
{
    Array *self   = malloc(sizeof(Array));
    self->free_fn = free_fn;
    self->buf     = malloc(sizeof(char *) * ARRAY_DEFAULT_SIZE);
    self->end     = 0;
    self->size    = ARRAY_DEFAULT_SIZE;

    return self;
}

void array_free(Array *self)
{
    if (!self)
        return;

    if (!(self->buf && self->free_fn))
        goto Skip;

    for (size_t i = 0; i < self->end; i++) {
        if (*(self->buf + i)) {
            self->free_fn(*(self->buf + i));
        }
    }

Skip:
    free(self);
}

void array_push(Array *self, void *value)
{
    // realloc for more space
    if (self->end == self->size)
        self->buf = realloc(self->buf, self->size * ARRAY_DEFAULT_SCALING);

    *(self->buf + self->end++) = value;
}

void array_pop(Array *self)
{
    if (self->end > 0)
        self->end--;
}
