#ifndef JLIB_STR_H
#define JLIB_STR_H

#include <stdlib.h>
#include <string.h>

int streq(const char *str0, const char *str1);
#ifndef strdup
char *strdup(const char *cstr);
#endif /* strdup */

/* use format modifiers to cat to a string with size reallocation
   implementation only accepts specifiers, using flags, widths,
   or precisions is undefined
   
   returns the cat'ed string */
char *strcatf(char *base, const char *fmt, ...);

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

void strcat_safe(char *destination, char *source);

#define _STRINGIFY(x) #x
#define STRINGIFY(x) _STRINGIFY(x)

#endif /* JLIB_STR_H */