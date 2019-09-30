#ifndef JLIB_VECTOR_H
#define JLIB_VECTOR_H

#include "allocator.h"
#include "generic.h" // ref

extern const unsigned int VECTOR_DEFAULT_SIZE;

typedef struct vector_tag {
    void *buf;
    // the sizeof an item in the buffer
    size_t data_size;
    // the max amount of items to go in the buffer
    size_t size;
    // the last (append) index of the buffer
    size_t end;
    allocator *alloc;
} vector;

#define vector(T) vector
#define vector_ref(T, pointer, index) ref(T, vector_at((pointer), (index)))

vector *vector_init(size_t data_size);
int vector_new(void *owner);
int vector_del(void *owner);

void *vector_at(vector *self, size_t index);
void vector_append(vector *self);

void vector_test();

#endif // JLIB_VECTOR_H