#ifndef JLIB_OBJECT_H
#define JLIB_OBJECT_H

#include "allocator.h"

typedef struct object_tab {
    void *buf;
    allocator *alloc;
} object;

object *object_init();
int object_new(void *buf);
int object_del(void *buf);

#endif // JLIB_OBJECT_H