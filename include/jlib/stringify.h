#ifndef JLIB_STRINGIFY_H
#define JLIB_STRINGIFY_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define streq(Str1, Str2) (strcmp(Str1, Str2) == 0)

// cpython's pymacro.h
#define _STRINGIFY(x) #x
#define STRINGIFY(x) _STRINGIFY(x)

/**
 * Strings to a given type
 */

#if __STDC_VERSION__ >= 201112L /* C11 support */

#define jlib_strtot(Target, String) \
    do { \
    Target = _Generic((Target), \
        char: strtol, \
        unsigned char: strtol, \
        int: strtol, \
        unsigned int: strtoul, \
        long int: strtol, \
        unsigned long int: strtoul, \
        long long int: strtoll, \
        unsigned long long int: strtoull, \
        float: strtof, \
        double: strtod, \
        long double: strtod, \
        default: strtol \
    )(String, /* endptr */ NULL, /* base */ 10); \
} while (0)

#endif // C11 support

/**
 * Heap allocated string shortcuts
 */

#define jlib_string_new(Size) calloc(Size, sizeof(char))
char *jlib_string_from(const char *literal);
int jlib_string_fast_eq(const char *str0, const char *str1);

char *jlib_string_fread(const char *fname);
void jlib_string_fwrite(const char *fname, const char *cstr);
void jlib_string_fappend(const char *fname, const char *cstr);

/**
 * Builder strings
 */

#define JLIB_STRING_MAX 256U
// option to use spaces between build strings
#ifndef JLIB_STRING_SEP
# define JLIB_STRING_SEP " "
#endif
void jlib_string_concat(char *target, void *src, size_t size);

#define JLIB_STRING_LOOKUP_PROTO(Type, Spec) \
    void _jlib_build_##Spec(char *string, Type value)

#define JLIB_STRING_LOOKUP(Type, Spec) \
inline void _jlib_build_##Spec(char *string, Type value) \
{ \
    char tmp[JLIB_STRING_MAX] = {0}; \
    snprintf(tmp, JLIB_STRING_MAX, "%" #Spec JLIB_STRING_SEP, value); \
    jlib_string_concat(string, tmp, strlen(tmp)); \
}

JLIB_STRING_LOOKUP_PROTO(char, c);
JLIB_STRING_LOOKUP_PROTO(int, d);
JLIB_STRING_LOOKUP_PROTO(unsigned int, u);
JLIB_STRING_LOOKUP_PROTO(long int, ld);
JLIB_STRING_LOOKUP_PROTO(unsigned long int, lu);
JLIB_STRING_LOOKUP_PROTO(long long int, lld);
JLIB_STRING_LOOKUP_PROTO(unsigned long long int, llu);
JLIB_STRING_LOOKUP_PROTO(float, f);
JLIB_STRING_LOOKUP_PROTO(double, lf);
JLIB_STRING_LOOKUP_PROTO(char *, s);

#if __STDC_VERSION__ >= 201112L /* C11 support */

#define jlib_strbuild(String, Value) \
    _Generic ((Value), \
        char: _jlib_build_c, \
        unsigned char: _jlib_build_c, \
        int: _jlib_build_d, \
        unsigned int: _jlib_build_u, \
        long int: _jlib_build_ld, \
        unsigned long int: _jlib_build_lu, \
        long long int: _jlib_build_lld, \
        unsigned long long int: _jlib_build_llu, \
        float: _jlib_build_f, \
        double: _jlib_build_lf, \
        long double: _jlib_build_lf, \
        char *: _jlib_build_s, \
        default: _jlib_build_d \
    )(String, Value)

#endif // C11 support

/**
 * Shortcut for catting a string with another string
 */
#define jlib_strcat(String1, String2) _jlib_build_s(String1, (char *)String2)

#endif // JLIB_STRINGIFY_H