#ifndef JLIB_ALLOC_H
#define JLIB_ALLOC_H

#include <stdlib.h>

#include "debug.h"

#ifndef JLIB_MALLOC
# define JLIB_MALLOC malloc
#endif

#ifndef JLIB_CALLOC
# define JLIB_CALLOC calloc
#endif

#ifndef JLIB_REALLOC
# define JLIB_REALLOC realloc
#endif

#ifndef JLIB_free
# define JLIB_FREE free
#endif

#define jlib_try_malloc(Target, Size, Errmsg) \
    do { \
        Target = JLIB_MALLOC(Size); \
        if (!Target) \
            jlib_halt(Errmsg); \
    } while (0)

#define jlib_try_calloc(Target, Count, Size, Errmsg) \
    do { \
        Target = JLIB_CALLOC(Count, Size); \
        if (!Target) \
            jlib_halt(Errmsg); \
    } while (0)

#define jlib_try_realloc(Block, Intermediate, Size, Errmsg) \
    do { \
        Intermediate = JLIB_REALLOC(Block, Size); \
        if (!Intermediate) \
            jlib_halt(Errmsg); \
        Block = Intermediate; \
    } while (0)

#define jlib_dealloc(Block) \
    do { \
        if (Block) \
            JLIB_DEALLOC(Block); \
    } while (0)

#define jlib_try_free(Block, Errmsg) \
    do { \
        if (Block) \
            JLIB_FREE(Block); \
        else \
            jlib_halt(Errmsg); \
    } while (0)

void *jlib_copy(void *buf, size_t size);

#endif // JLIB_ALLOC_H