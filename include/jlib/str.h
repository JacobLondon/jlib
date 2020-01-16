#ifndef JLIB_STR_H
#define JLIB_STR_H

#include <stdlib.h>
#include <string.h>

int streq(const char *str0, const char *str1);
#ifndef strdup
char *strdup(const char *cstr);
#endif /* strdup */

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

#endif /* JLIB_STR_H */