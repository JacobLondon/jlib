#ifndef JLIB_STR_H
#define JLIB_STR_H

#include <stddef.h>
#include <string.h>

/**
 * \brief Use printf modifiers to cat to a string with size reallocation.
 * Implementation only accepts specifiers, using flags, widths,
 * or precisions will exit with an error.
 * \param base Only pass NULL or the string to return to
 * \return the cat'ed string or NULL if alloc'ing fails
 * 
 * Usage:
 *   char *new = strcatf(NULL, "%d", 100);
 *     or
 *   char *build = strdup("Lorem");
 *   build = strcatf(build, "%s", "Ipsum");
 * Else:
 *    If base is a different string than the return value,
 *    then \a base will point to an invalid address (due to realloc)
 */
int strcatf(char **buffer, const char *format, ...);
#if 0
/* find the length of the string equivalent of the number */
size_t strfmtlen_d(long long number);  /* signed integer */
#define strfmtlen_i strfmtlen_d
size_t strfmtlen_u(unsigned long long number); /* unsigned, intmax, size_t, ptrdiff */
#define strfmtlen_j strfmtlen_u
#define strfmtlen_z strfmtlen_u
#define strfmtlen_t strfmtlen_u
size_t strfmtlen_o(unsigned long long number); /* octal */
size_t strfmtlen_x(unsigned long long number); /* hex */
#define strfmtlen_p strfmtlen_x /* pointer is hex */

size_t strfmtlen_f(float number); /* float 32, assume IEEE 754 and Little Endian */
size_t strfmtlen_lf(double number); /* float 64, assume IEEE 754 and Little Endian */
size_t strfmtlen_e(double number); /* scientific notation, ignore shortest notation */
#define strfmtlen_g strfmtlen_e
size_t strfmtlen_a(double number); /* hexadecimal floating point */
#endif
int strcat_safe(char *destination, char *source);

int streq(const char *str0, const char *str1);
#ifndef strdup
char *strdup(const char *str);
#endif /* strdup */
char *strndup(const char *str, size_t n);

/** 
 * Split a string by the fmt string.
 * All characters in the fmt string will not be included
 * in the result.
 * 
 * Return a NULL terminated array of strings
 */
char **strsplit(char *s, const char *fmt);
void strsplit_free(char **buf);

#define _STRINGIFY(x) #x
#define STRINGIFY(x) _STRINGIFY(x)

#endif /* JLIB_STR_H */