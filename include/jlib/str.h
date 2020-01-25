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
/* TODO: eE gG aA */

#define _STRINGIFY(x) #x
#define STRINGIFY(x) _STRINGIFY(x)

/**
 * Builder strings
 */

#define STRCAT_MAX 128U /* number value unlikely to have more digits */

#define STRCAT_LOOKUP_PROTO(Type, Spec) \
	void strcat_##Spec(char *string, Type value)

#define STRCAT_LOOKUP(Type, Spec) \
void strcat_##Spec(char *dest, Type value) \
{ \
	char tmp[STRCAT_MAX] = {0}; \
	sprintf(tmp, "%" #Spec " ", value); \
	strncat(dest, tmp, strlen(tmp)); \
}

STRCAT_LOOKUP_PROTO(char, c);
STRCAT_LOOKUP_PROTO(int, d);
STRCAT_LOOKUP_PROTO(unsigned int, u);
STRCAT_LOOKUP_PROTO(long int, ld);
STRCAT_LOOKUP_PROTO(unsigned long int, lu);
STRCAT_LOOKUP_PROTO(long long int, lld);
STRCAT_LOOKUP_PROTO(unsigned long long int, llu);
STRCAT_LOOKUP_PROTO(float, f);
STRCAT_LOOKUP_PROTO(double, lf);
void strcat_safe(char *destination, char *source);

#endif /* JLIB_STR_H */