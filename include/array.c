#include <stdlib.h>
#include <stdio.h>

#include "array.h"
#include "debug.h"

Array *array_new(void (* f_free)(void *buf))
{
    Array *self;
    if (!(self = malloc(sizeof(struct array_s))))
        halt("Could not malloc for array initialization");
    if (!(self->buf = malloc(sizeof(void *) * ARRAY_DEFAULT_CAP)))
        halt("Could not malloc array buffer");
    self->size = 0;
    self->cap  = ARRAY_DEFAULT_CAP;
    self->free = f_free;

    return self;
}

void array_free(Array *self)
{
    if (!self)
        return;

    if (!(self->buf && self->free))
        goto Skip;

    for (size_t i = 0; i < self->size; i++) {
        if (*(self->buf + i)) {
            self->free(*(self->buf + i));
        }
    }

Skip:
    free(self);
}

void array_push_(Array *self, void *value)
{
    // realloc for more space
    if (self->size == self->cap) {
        self->cap = self->cap * ARRAY_DEFAULT_SCALING;
        array_resize(self, self->cap);
    }

    *(self->buf + self->size++) = value;
}

void array_pop(Array *self)
{
    if (self->size > 0)
        self->size--;
}

void array_resize(Array *self, size_t size)
{
    if (!(self->buf = realloc(self->buf, size)))
        halt("Could not realloc array buffer");
}
