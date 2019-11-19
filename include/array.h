#ifndef JLIB_ARRAY_H
#define JLIB_ARRAY_H

#include <stdlib.h> // size_t

#define ARRAY_DEFAULT_SIZE 8
#define ARRAY_DEFAULT_SCALING 2

typedef struct array_s {
    void (* free_fn)(void *buf);
    void **buf;
    size_t end;
    size_t size;
} Array;

struct array_fn_s {
    Array *(* new)(void (* free_fn)(void *buf));
    void (* free)(Array *self);
    void (* push)(Array *self, void *value);
    void (* pop)(Array *self);
};

Array *array_new(void (* free_fn)(void *buf));
void array_free(Array *self);
void array_push(Array *self, void *value);
void array_pop(Array *self);

#endif // JLIB_ARRAY_H