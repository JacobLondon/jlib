#include "stringify.h"

char *hstring_lit(const char *literal)
{
    size_t size = strlen(literal);
    char *build = hstring(size + 1);
    size_t i = 0;
    while ((build[i] = literal[i]))
        i++;
    
    return build;
}

void build_concat(char *htarget, void *src, size_t size)
{
    size_t start = strlen(htarget);
    size_t length = start + size + 1;
    htarget = realloc(htarget, sizeof(char) * length);
    for (size_t i = 0; i < length; i++) {
        htarget[start + i] = ((char *)src)[i];
    }
}
