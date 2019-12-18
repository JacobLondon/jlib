#ifndef JLIB_STRINGIFY_H
#define JLIB_STRINGIFY_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define streq(Str1, Str2) (strcmp(Str1, Str2) == 0)

/**
 * Strings to a given type
 */

#define strtot(Target, String) do {              \
    Target = _Generic((Target),                  \
        char: strtol,                            \
        unsigned char: strtol,                   \
        int: strtol,                             \
        unsigned int: strtoul,                   \
        long int: strtol,                        \
        unsigned long int: strtoul,              \
        long long int: strtoll,                  \
        unsigned long long int: strtoull,        \
        float: strtof,                           \
        double: strtod,                          \
        long double: strtod,                     \
        default: build_d                         \
    )(String, /* endptr */ NULL, /* base */ 10); \
} while (0)

/**
 * Heap allocated string shortcuts
 */

#define hstring(Size) calloc(Size, sizeof(char))
char *hstring_lit(const char *literal);

/**
 * Builder strings
 */

#define BUILD_MAX 256U
// option to use spaces between build strings
#ifndef BUILD_SEP
# define BUILD_SEP " "
#endif
void build_concat(char *target, void *src, size_t size);

#define BUILD_LOOKUP(Type, Spec)                          \
inline void build_##Spec(char *string, Type value)        \
{                                                         \
    char tmp[BUILD_MAX] = {0};                            \
    snprintf(tmp, BUILD_MAX, "%" #Spec BUILD_SEP, value); \
    build_concat(string, tmp, strlen(tmp));               \
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

#define buildstr(String, Value)            \
    _Generic ((Value),                     \
        char: build_c,                     \
        unsigned char: build_c,            \
        int: build_d,                      \
        unsigned int: build_u,             \
        long int: build_ld,                \
        unsigned long int: build_lu,       \
        long long int: build_lld,          \
        unsigned long long int: build_llu, \
        float: build_f,                    \
        double: build_lf,                  \
        long double: build_lf,             \
        char *: build_s,                   \
        default: build_d                   \
    )(String, Value)

/**
 * Shortcut for catting a string with another string
 */
#define buildcat(String1, String2) build_s(String1, (char *)String2)

#endif // JLIB_STRINGIFY_H