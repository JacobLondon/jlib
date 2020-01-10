#ifndef JLIB_STRINGIFY_H
#define JLIB_STRINGIFY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *strnew(const char *literal);
int streq(const char *str0, const char *str1);

char *file_read(const char *fname);
void file_write(const char *fname, const char *cstr);
void file_append(const char *fname, const char *cstr);

#define _STRINGIFY(x) #x
#define STRINGIFY(x) _STRINGIFY(x)

/**
 * Builder strings
 */

#define CONCAT_MAX 128U /* number value unlikely to have more digits */

#define CONCAT_LOOKUP_PROTO(Type, Spec) \
	void concat_##Spec(char *string, Type value)

#define CONCAT_LOOKUP(Type, Spec) \
void concat_##Spec(char *dest, Type value) \
{ \
	char tmp[CONCAT_MAX] = {0}; \
	sprintf(tmp, "%" #Spec " ", value); \
	strncat(dest, tmp, strlen(tmp)); \
}

CONCAT_LOOKUP_PROTO(char, c);
CONCAT_LOOKUP_PROTO(int, d);
CONCAT_LOOKUP_PROTO(unsigned int, u);
CONCAT_LOOKUP_PROTO(long int, ld);
CONCAT_LOOKUP_PROTO(unsigned long int, lu);
CONCAT_LOOKUP_PROTO(long long int, lld);
CONCAT_LOOKUP_PROTO(unsigned long long int, llu);
CONCAT_LOOKUP_PROTO(float, f);
CONCAT_LOOKUP_PROTO(double, lf);

#endif /* JLIB_STRINGIFY_H */