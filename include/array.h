#ifndef JLIB_ARRAY_H
#define JLIB_ARRAY_H

#include <stdlib.h> // size_t

#include "generic.h"

#define ARRAY_DEFAULT_CAP 8
#define ARRAY_DEFAULT_SCALING 2

struct array_s {
    void **buf;
    size_t size;
    size_t cap;
    void (* free)(void *buf);
};

typedef struct array_s Array;

Array *array_new(void (* f_free)(void *buf));
void array_free(Array *self);
void array_push_(Array *self, void *value);
void array_pop(Array *self);
void array_resize(Array *self, size_t size);

#define array_push(arrayptr, valueptr) array_push_((arrayptr), ref((valueptr)))
#define array_read(arrayptr, index, type) (val((arrayptr)->buf[(index)], type))
#define array_write(arrayptr, index, valueptr) do { (arrayptr)->buf[(index)] = ref((valueptr)); } while (0)

#endif // JLIB_ARRAY_H