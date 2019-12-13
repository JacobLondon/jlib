#ifndef JLIB_STRINGIFY_H
#define JLIB_STRINGIFY_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define streq(str1, str2) (strcmp(str1, str2) == 0)

/**
 * Heap allocated string shortcuts
 */

#define hstring(size) calloc(size, sizeof(char))
char *hstring_lit(const char *literal);

/**
 * Builder strings
 */

#define BUILD_MAX 256U
void build_concat(char *target, void *src, size_t size);

#define BUILD_LOOKUP(type, spec)                   \
inline void build_##spec(char *string, type value) \
{                                                  \
    char tmp[BUILD_MAX] = {0};                     \
    snprintf(tmp, BUILD_MAX, "%" #spec, value);    \
    build_concat(string, tmp, strlen(tmp));        \
}

BUILD_LOOKUP(char, c)
BUILD_LOOKUP(int, d)
BUILD_LOOKUP(unsigned int, u)
BUILD_LOOKUP(long int, ld)
BUILD_LOOKUP(unsigned long int, lu)
BUILD_LOOKUP(long long int, lld)
BUILD_LOOKUP(unsigned long long int, llu)
BUILD_LOOKUP(float, f)
BUILD_LOOKUP(double, lf)
BUILD_LOOKUP(char *, s)

#define buildstr(string, value)            \
    _Generic ((value),                     \
        char: build_c,                     \
        int: build_d,                      \
        unsigned int: build_u,             \
        long int: build_ld,                \
        unsigned long int: build_lu,       \
        long long int: build_lld,          \
        unsigned long long int: build_llu, \
        float: build_f,                    \
        double: build_d,                   \
        char *: build_s                    \
    )(string, value)

/**
 * Shortcut for catting a string with another string
 */
#define buildcat(string1, string2) build_s(string1, (char *)string2)

#endif // JLIB_STRINGIFY_H