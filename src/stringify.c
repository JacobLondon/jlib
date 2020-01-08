#include <jlib/stringify.h>
#include <jlib/alloc.h>

char *jlib_string_from(const char *literal)
{
    size_t size = strlen(literal);
    char *build = jlib_string_new(size + 1);
    size_t i = 0;
    while ((build[i] = literal[i]))
        i++;
    
    return build;
}

void jlib_string_concat(char *htarget, void *src, size_t size)
{
    size_t start = strlen(htarget);
    size_t length = start + size + 1;
    
    // realloc characters
    char *tmp;
    jlib_try_realloc(htarget, tmp, length, "Coult not realloc in build_concat");

    for (size_t i = 0; i < size; i++) {
        htarget[start + i] = ((char *)src)[i];
    }
}

#if __STDC_VERSION__ >= 201112L /* C11 support */

JLIB_STRING_LOOKUP(char, c)
JLIB_STRING_LOOKUP(int, d)
JLIB_STRING_LOOKUP(unsigned int, u)
JLIB_STRING_LOOKUP(long int, ld)
JLIB_STRING_LOOKUP(unsigned long int, lu)
JLIB_STRING_LOOKUP(long long int, lld)
JLIB_STRING_LOOKUP(unsigned long long int, llu)
JLIB_STRING_LOOKUP(float, f)
JLIB_STRING_LOOKUP(double, lf)
JLIB_STRING_LOOKUP(char *, s)

#endif
