#include <stdlib.h>
#include <stdio.h>

#include <jlib/array.h>
#include <jlib/alloc.h>

Array *jlib_array_new(void (* f_free)(void *buf))
{
    Array *self;
    jlib_try_malloc(self, sizeof(struct jlib_array_t), "Could not malloc for array initialization");
    jlib_try_malloc(self->buf, sizeof(void *) * JLIB_ARRAY_DEFAULT_CAP, "Could not malloc array buffer");
    self->size = 0;
    self->cap  = JLIB_ARRAY_DEFAULT_CAP;
    self->free = f_free;

    return self;
}

void jlib_array_free(Array *self)
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

void jlib_array_push_(Array *self, void *value)
{
    // realloc for more space
    if (self->size == self->cap) {
        self->cap = self->cap * JLIB_ARRAY_DEFAULT_SCALING;
        jlib_array_resize(self, self->cap);
    }

    *(self->buf + self->size++) = value;
}

void jlib_array_pop(Array *self)
{
    if (self->size > 0)
        self->size--;
}

void jlib_array_resize(Array *self, size_t size)
{
    void **tmp;
    jlib_try_realloc(self->buf, tmp, size, "Could not realloc array buffer");
}
