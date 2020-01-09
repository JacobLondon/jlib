#include <stdlib.h>

#include <jlib/util.h>

void jlib_swap(void *x, void *y)
{
    x = (void *)((size_t)x ^ (size_t)y);
    y = (void *)((size_t)y ^ (size_t)x);
    x = (void *)((size_t)x ^ (size_t)y);
}

void jlib_clear(void *buf, int bytes)
{
    for (int i = 0; i < bytes; i++)
        ((char *)buf)[i] = 0;
}
