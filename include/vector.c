#include <stdio.h>
#include <stdlib.h>
#include "vector.h"

const unsigned int VECTOR_DEFAULT_SIZE = 8;

vector *vector_init(size_t data_size)
{
    vector *self = malloc(sizeof(vector));
    self->data_size = data_size;

    self->alloc = allocator_init(vector_new, vector_del);
    allocate(self);
    return self;
}

int vector_new(void *owner)
{
    vector *self = (vector *)owner;
    self->buf  = calloc(VECTOR_DEFAULT_SIZE, self->data_size);
    self->size = VECTOR_DEFAULT_SIZE;
    self->end  = 0;

    return 1;
}

int vector_del(void *owner)
{
    if (!owner)
        return 1;
    
    vector *self = (vector *)self->buf;
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

void *vector_at(vector *self, size_t index)
{
    return (void *)(self->buf + self->data_size * index);
}

void vector_append(vector *self)
{

}

/**
 * test
 */

void vector_test()
{
    vector(int) *myvec = vector_init(sizeof(int));
    vector_ref(int, myvec, 0) = 10;
    printf("%d\n", vector_ref(int, myvec, 0));

    delete(myvec);
}

