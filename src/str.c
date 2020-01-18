#include <stdarg.h>
#include <stdio.h>

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

char *strcatf(char *dest, const char *fmt, ...)
{
	size_t destlen;
	size_t fmtlen = strlen(fmt);
	if (dest)
		destlen = strlen(dest);
	else
		destlen = 0;

	va_list arglist;
	va_start(arglist, fmt);

	size_t i, cap;
	char *tmp;
	char ch;
	for (i = 0, cap = destlen; i < fmtlen; i++) {
		if (fmt[i] == '%') {
			printf("Here 1\n");
			ch = fmt[i + 1];
			printf("Here %c\n", ch);
			switch(ch) {
			/* modifiers */
			case 'h': case 'l': case 'j': case 'z': case 't':
			/* integers */
			case 'd': case 'i': case 'u': case 'o':
			case 'x': case 'X': case 'p':
			/* floats */
			case 'f': case 'F': case 'e': case 'E':
			case 'g': case 'G': case 'a': case 'A':
				cap += 17;
				i++;
				break;
			/* single chars */
			case 'c': case '%':
				cap++;
				i++;
				break;
			/* nothing */
			case 'n':
				i++;
				break;
			/* strings */
			case 's':
				tmp = va_arg(arglist, char *);
				cap += strlen(tmp);
				i++;
				break;
			default:
				i++;
				break;
			}
			printf("Here 3\n");
		} else
			cap++;
	}
	cap++;
	va_end(arglist);

	if (dest)
		tmp = (char *)realloc(dest, cap + 1);
	else
		tmp = (char *)calloc(cap + 1, sizeof(char));

	if (!tmp) {
		fputs("Error: Could not realloc in strcatf", stderr);
		exit(-1);
	}
	dest = tmp;

	va_start(arglist, fmt);
	vsnprintf(&dest[destlen], cap - destlen, fmt, arglist);
	va_end(arglist);

	/* cut off extraneous 0's */
	printf("Got here\n");
	/*size_t smallen = strlen(dest);
	tmp = (char *)realloc(dest, smallen + 1);
	if (!tmp) {
		fputs("Error: Could not shrink in strcatf", stderr);
		exit(-1);
	}
	dest = tmp;*/
	/*dest[cap + 1] = 0; /* NULL term */

	return dest;
}

STRCAT_LOOKUP(char, c)
STRCAT_LOOKUP(int, d)
STRCAT_LOOKUP(unsigned int, u)
STRCAT_LOOKUP(long int, ld)
STRCAT_LOOKUP(unsigned long int, lu)
STRCAT_LOOKUP(long long int, lld)
STRCAT_LOOKUP(unsigned long long int, llu)
STRCAT_LOOKUP(float, f)
STRCAT_LOOKUP(double, lf)

void strcat_safe(char *destination, char *source)
{
	void *tmp = realloc(destination, strlen(destination) + strlen(source) + 1);
	if (!tmp) {
		fputs("Error: Could not realloc in strcat_safe", stderr);
		exit(-1);
	}
	strcat(destination, source);
}