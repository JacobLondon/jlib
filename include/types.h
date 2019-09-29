#ifndef JLIB_TYPES_H
#define JLIB_TYPES_H

#include "object.h"

typedef struct hvector_tag {
    void *buf;
    // the sizeof an item in the buffer
    size_t data_size;
    // the max amount of items to go in the buffer
    size_t size;
    // the last (append) index of the buffer
    size_t end;
} hvector;

typedef object vector;
#define vector(T) vector

#endif // JLIB_TYPES_H