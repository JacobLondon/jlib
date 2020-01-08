#include <jlib/alloc.h>

void *jlib_copy(void *buf, size_t size)
{
    char *new;
    jlib_try_malloc(new, size, "Could not malloc for copying");
    for (size_t i = 0; i < size; i++)
        new[i] = ((char *)buf)[i];
    return (void *)new;
}
