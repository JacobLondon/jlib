#ifndef JLIB_OWNED_H
#define JLIB_OWNED_H

#include "allocator.h"

typedef struct owned_tab {
    void *buf;
    allocator *alloc;
} owned;

owned *owned_init();
int owned_new(void *owner);
int owned_del(void *owner);

#endif // JLIB_OWNED_H