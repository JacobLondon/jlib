#include <stdio.h>

#include "debug.h"
#include "generic.h"

void *copy(void *buf, size_t size)
{
    char *new;
    if (!(new = malloc(size))) halt("Could not malloc for copying");
    for (size_t i = 0; i < size; i++)
        new[i] = ((char *)buf)[i];
    return (void *)new;
}

void delete(void *buf, void (*f_free)(void *buf))
{
    if (buf) {
        if (f_free)
            f_free(buf);
        else
            free(buf);
        buf = NULL;
    }
}
