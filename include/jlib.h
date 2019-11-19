#ifndef JLIB_JLIB_H
#define JLIB_JLIB_H

#include "allocator.h"
#include "array.h"
#include "debug.h"
#include "generic.h"
#include "match.h"
#include "print.h"

typedef struct jlib_s {
    struct array_fn_s array;
} JLib;

int jlib_init();

extern JLib jlib;

#endif // JLIB_JLIB_H