#ifndef JLIB_ARRAY_H
#define JLIB_ARRAY_H

#include <stdlib.h> // size_t

#include "generic.h"

#define JLIB_ARRAY_DEFAULT_CAP 8
#define JLIB_ARRAY_DEFAULT_SCALING 2

struct jlib_array_t {
    void **buf;
    size_t size;
    size_t cap;
    void (* free)(void *buf);
};

#ifdef Array
    #warning "Warning: Identifier 'Array' already defined, cannot typedef 'struct jlib_array_t' to 'Array'"
#else
    typedef struct jlib_array_t Array;
#endif

Array *jlib_array_new(void (* f_free)(void *buf));
void jlib_array_free(Array *self);
void jlib_array_push_(Array *self, void *value);
void jlib_array_pop(Array *self);
void jlib_array_resize(Array *self, size_t size);

#define jlib_array_push(Arrayptr, Valueptr) jlib_array_push_((Arrayptr), jlib_ref((Valueptr)))
#define jlib_array_read(Arrayptr, Index, Type) (jlib_val((Arrayptr)->buf[(Index)], Type))
#define jlib_array_write(Arrayptr, Index, Valueptr) do { (Arrayptr)->buf[(Index)] = jlib_ref((Valueptr)); } while (0)

#endif // JLIB_ARRAY_H