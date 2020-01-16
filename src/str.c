#include <jlib/str.h>

int streq(const char *cstr0, const char *cstr1)
{
	size_t i;
	for (i = 0; cstr0[i] || cstr1[i]; i++) {
		if (cstr0[i] != cstr1[i])
			return 0;
	}
	return 1;
}

#ifndef strdup
char *strdup(const char *cstr)
{
	char *build = calloc(strlen(cstr) + 1, sizeof(char));
	if (!build) {
		fputs("Error: Could not calloc in strnew\n", stderr);
		exit(-1);
	}
	size_t i;
	for (i = 0; (build[i] = cstr[i]); i++)
		;
	
	return build;
}
#endif /* strdup */

STRCAT_LOOKUP(char, c)
STRCAT_LOOKUP(int, d)
STRCAT_LOOKUP(unsigned int, u)
STRCAT_LOOKUP(long int, ld)
STRCAT_LOOKUP(unsigned long int, lu)
STRCAT_LOOKUP(long long int, lld)
STRCAT_LOOKUP(unsigned long long int, llu)
STRCAT_LOOKUP(float, f)
STRCAT_LOOKUP(double, lf)
