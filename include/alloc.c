#include "alloc.h"

void *copy(void *buf, size_t size)
{
    char *new;
    if (!(new = malloc(size))) halt("Could not malloc for copying");
    for (size_t i = 0; i < size; i++)
        new[i] = ((char *)buf)[i];
    return (void *)new;
}
