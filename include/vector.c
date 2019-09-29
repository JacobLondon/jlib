#include <stdlib.h>
#include "vector.h"

const unsigned int VECTOR_DEFAULT_SIZE = 8;

vector *vector_init(size_t data_size)
{
    vector *self = object_init();
    self->buf = malloc(sizeof(hvector));

    // container data size
    hvector *vec = (hvector *)self->buf;
    vec->data_size = data_size;

    self->alloc = allocator_init(vector_new, vector_del);
    allocate(self);
    return self;
}

int vector_new(void *buf)
{
    vector *self = (vector *)buf;
    hvector *vec = (hvector *)self->buf;
    vec->buf  = calloc(VECTOR_DEFAULT_SIZE, vec->data_size);
    vec->size = VECTOR_DEFAULT_SIZE;
    vec->end  = 0;

    return 1;
}

int vector_del(void *buf)
{
    vector *self = (vector *)buf;
    hvector *vec = (hvector *)self->buf;

    if (vec->buf)
        free(vec->buf);
    
    vec->buf = NULL;
    allocator_free(self->alloc);
    free(self->buf);
    self->buf = NULL;
    free(buf);
    buf = NULL;

    return 1;
}

void *vector_at(vector *self, size_t index)
{
    hvector *vec = (hvector *)self->buf;
    
    return (void *)(vec->buf + vec->data_size * index);
}

void vector_append(vector *self)
{

}
