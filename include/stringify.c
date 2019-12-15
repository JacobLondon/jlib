#include "stringify.h"
#include "debug.h"

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
    // realloc characters
    char *tmp;
    if (!(tmp = realloc(htarget, length)))
        halt("Could not realloc in build_concat");
    htarget = tmp;
    for (size_t i = 0; i < size; i++) {
        htarget[start + i] = ((char *)src)[i];
    }
}
