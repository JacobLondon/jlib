#ifndef JLIB_ALLOC_H
#define JLIB_ALLOC_H

#include <stdlib.h>

#include "debug.h"

#ifndef DEF_MALLOC
# define DEF_MALLOC malloc
#endif

#ifndef DEF_CALLOC
# define DEF_CALLOC calloc
#endif

#ifndef DEF_REALLOC
# define DEF_REALLOC realloc
#endif

#ifndef DEF_DEALLOC
# define DEF_DEALLOC free
#endif

#define try_malloc(Target, Size, Errmsg) do { \
    Target = DEF_MALLOC(Size);                \
    if (!Target)                              \
        halt(Errmsg);                         \
} while (0)

#define try_calloc(Target, Count, Size, Errmsg) do { \
    Target = DEF_CALLOC(Count, Size);                \
    if (!Target)                                     \
        halt(Errmsg);                                \
} while (0)

#define try_realloc(Block, Intermediate, Size, Errmsg) do { \
    Intermediate = DEF_REALLOC(Block, Size);                \
    if (!Intermediate)                                      \
        halt(Errmsg);                                       \
    Block = Intermediate;                                   \
} while (0)

#define dealloc(Block) do { \
    if (Block)              \
        DEF_DEALLOC(Block); \
} while (0)

#define try_dealloc(Block, Errmsg) do { \
    if (Block)                          \
        DEF_DEALLOC(Block);             \
    else                                \
        halt(Errmsg);                   \
} while (0)



void *copy(void *buf, size_t size);

#endif // JLIB_ALLOC_H