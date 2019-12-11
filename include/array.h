#ifndef JLIB_ARRAY_H
#define JLIB_ARRAY_H

#include <stdlib.h> // size_t

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
void array_push(Array *self, void *value);
void array_pop(Array *self);
void array_resize(Array *self, size_t size);

#endif // JLIB_ARRAY_H