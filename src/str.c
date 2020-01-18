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
	va_list arglist;
	va_start(arglist, fmt);

	/* allow for NULL dest, create a new string */
	if (dest)
		destlen = strlen(dest);
	else
		destlen = 0;

	/* record the number of bytes needed to hold the formatted string */
	size_t i, bytes;
	char *tmp;
	for (i = 0, bytes = destlen; i < fmtlen; i++) {
		if (fmt[i] == '%') {
			switch(fmt[i + 1]) {
			/* short */
			case 'h': {
				switch (fmt[i + 2]) {
				/* char: hh[diuoxX] */
				case 'h': {
					switch (fmt[i + 3]) {
					case 'd': case 'i': case 'u':
					case 'o': case 'x': case 'X':
						(void)va_arg(arglist, int); i++;
						break;
					default:
						goto Error;
					}
					break;
				}
				/* short int: h[diuoxX] */
				case 'd': case 'i': case 'u':
				case 'o': case 'x': case 'X':
					(void)va_arg(arglist, int); i++; 
					break;
				default:
					goto Error;
				}
				i++;
				goto Increment;
			} /* end short */

			/* long */
			case 'l': {
				switch (fmt[i + 2]) {
				/* long long int: ll[diuoxX] */
				case 'l': {
					switch (fmt[i + 3]) {
					case 'd': case 'i': case 'u':
					case 'o': case 'x': case 'X':
						(void)va_arg(arglist, long long); i++;
						break;
					default:
						goto Error;
					}
					break;
				}
				/* double: lf */
				case 'f': case 'F':
					(void)va_arg(arglist, double); i++;
					break;
				/* long int: l[diuoxX] */
				case 'd': case 'i': case 'u':
				case 'o': case 'x': case 'X':
					(void)va_arg(arglist, long); i++;
					break;
				default:
					goto Error;
				}
				i++;
				goto Increment;
			} /* end long */

			/* floats */
			case 'f': case 'F': case 'e': case 'E':
			case 'g': case 'G': case 'a': case 'A':
				(void)va_arg(arglist, double); i++;
				goto Increment;
			/* long double */
			case 'L':
				(void)va_arg(arglist, long double); i++;
				goto Increment;
			/* int */
			case 'd': case 'i':
				(void)va_arg(arglist, int); i++;
				goto Increment;
			/* unsigned int */
			case 'u': case 'o': case 'x': case 'X':
				(void)va_arg(arglist, unsigned); i++;
				goto Increment;
			case 'p': /* pointer */
			case 'j': /* intmax_t or uintmax_t */
			case 'z': /* size_t */
			case 't': /* ptrdiff_t */
				(void)va_arg(arglist, size_t); i++;
				goto Increment;

			Increment:
				bytes += 17;
				break;
			/* single chars */
			case 'c': case '%':
				(void)va_arg(arglist, int);
				bytes++; i++;
				break;
			/* nothing */
			case 'n':
				(void)va_arg(arglist, int);
				i++;
				break;
			/* strings */
			case 's':
				tmp = va_arg(arglist, char *);
				bytes += strlen(tmp);
				i++;
				break;
			default:
				goto Error;
			}
		} else /* end if('%'), is a regular character */
			bytes++;
	}
	bytes++;
	va_end(arglist);

	if (dest)
		tmp = (char *)realloc(dest, bytes + 1);
	else
		tmp = (char *)calloc(bytes + 1, sizeof(char));

	if (!tmp) {
		fputs("Error: Could not realloc in strcatf", stderr);
		exit(-1);
	}
	dest = tmp;

	/* append to the format string */
	va_start(arglist, fmt);
	vsnprintf(&dest[destlen], bytes - destlen, fmt, arglist);
	va_end(arglist);

	/* cut off extraneous 0's */
	destlen = strlen(dest);
	tmp = (char *)realloc(dest, destlen + 1);
	if (!tmp) {
		fputs("Error: Could not shrink in strcatf", stderr);
		exit(-1);
	}
	dest = tmp;
	dest[destlen + 1] = 0; /* NULL term */

	return dest;
Error:
	fputs("Error: Invalid strcatf format specifier", stderr);
	exit(-1);
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