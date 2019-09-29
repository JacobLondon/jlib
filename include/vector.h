#ifndef JLIB_VECTOR_H
#define JLIB_VECTOR_H

#include "types.h"

extern const unsigned int VECTOR_DEFAULT_SIZE;

vector *vector_init(size_t data_size);
int vector_new(void *buf);
int vector_del(void *buf);

void *vector_at(vector *self, size_t index);
void vector_append(vector *self);

#endif // JLIB_VECTOR_H