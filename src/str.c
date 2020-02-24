#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
//#include <math.h>

#include <jlib/str.h>

enum { BUF_LEN = 512 }; /* DBL_MAX's length = 316 characters, go to 512 for good measure */

static char _InternalFmtBuf[BUF_LEN];
static void clear_fmt_buf()
{
	int i = 0;
	for (; i < BUF_LEN; i++) {
		/* unlikely */
		if (__builtin_expect(_InternalFmtBuf[i] == 0, 0))
			break;
		else
			_InternalFmtBuf[i] = 0;
	}
}

int streq(const char *str0, const char *str1)
{
	size_t i;
	for (i = 0; str0[i] || str1[i]; i++) {
		if (str0[i] != str1[i])
			return 0;
	}
	return 1;
}

#ifndef strdup
char *strdup(const char *cstr)
{
	char *build = calloc(strlen(cstr) + 1, sizeof(char));
	if (!build) {
		return NULL;
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
					case 'd':
					case 'i': bytes += strfmtlen_d(va_arg(arglist, int)); break;
					case 'u': bytes += strfmtlen_u(va_arg(arglist, unsigned)); break;
					case 'o': bytes += strfmtlen_o(va_arg(arglist, unsigned)); break;
					case 'x':
					case 'X': bytes += strfmtlen_x(va_arg(arglist, int)); break;
					default: goto Error;
					}
					i++;
					break;
				}
				/* short int: h[diuoxX] */
				case 'd':
				case 'i': bytes += strfmtlen_d(va_arg(arglist, int)); break;
				case 'u': bytes += strfmtlen_u(va_arg(arglist, unsigned)); break;
				case 'o': bytes += strfmtlen_o(va_arg(arglist, unsigned)); break;
				case 'x':
				case 'X': bytes += strfmtlen_x(va_arg(arglist, int)); break;
				default: goto Error;
				}
				i++;
				break;
			} /* end short */

			/* long */
			case 'l': {
				switch (fmt[i + 2]) {
				/* long long int: ll[diuoxX] */
				case 'l': {
					switch (fmt[i + 3]) {
					case 'd':
					case 'i': bytes += strfmtlen_d(va_arg(arglist, long long)); break;
					case 'u': bytes += strfmtlen_u(va_arg(arglist, unsigned long long)); break;
					case 'o': bytes += strfmtlen_o(va_arg(arglist, unsigned long long)); break;
					case 'x':
					case 'X': bytes += strfmtlen_x(va_arg(arglist, unsigned long long)); break;
					default: goto Error;
					}
					i++;
					break;
				}
				/* double: lf */
				case 'f':
				case 'F': bytes += strfmtlen_lf(va_arg(arglist, double)); break;
				/* long int: l[diuoxX] */
				case 'd':
				case 'i': bytes += strfmtlen_d(va_arg(arglist, long)); break;
				case 'u': bytes += strfmtlen_u(va_arg(arglist, unsigned long)); break;
				case 'o': bytes += strfmtlen_o(va_arg(arglist, unsigned long)); break;
				case 'x':
				case 'X': bytes += strfmtlen_x(va_arg(arglist, unsigned long)); break;
				default: goto Error;
				}
				i++;
				break;
			} /* end long */

			/* floats */
			case 'f':
			case 'F': bytes += strfmtlen_f(va_arg(arglist, double)); break;
			case 'e':
			case 'E': bytes += strfmtlen_e(va_arg(arglist, double)); break;
			case 'g':
			case 'G': bytes += strfmtlen_g(va_arg(arglist, double)); break;
			case 'a':
			case 'A': bytes += strfmtlen_a(va_arg(arglist, double)); break;
			/* long double */
			case 'L': bytes += strfmtlen_lf(va_arg(arglist, long double)); break;
			/* int */
			case 'd':
			case 'i': bytes += strfmtlen_d(va_arg(arglist, int)); break;
			/* unsigned int */
			case 'u': bytes += strfmtlen_u(va_arg(arglist, unsigned)); break;
			case 'o': bytes += strfmtlen_o(va_arg(arglist, unsigned)); break;
			case 'x':
			case 'X': bytes += strfmtlen_x(va_arg(arglist, unsigned)); break;
			/* pointer */
			case 'p': bytes += strfmtlen_p(va_arg(arglist, size_t)); break;
			/* intmax_t or uintmax_t */
			case 'j': bytes += strfmtlen_j(va_arg(arglist, size_t)); break;
			/* size_t */
			case 'z': bytes += strfmtlen_z(va_arg(arglist, size_t)); break;
			/* ptrdiff_t */
			case 't': bytes += strfmtlen_t(va_arg(arglist, size_t)); break;
			/* single chars */
			case 'c':
			case '%':
				(void)va_arg(arglist, int);
				bytes++;
				break;
			/* nothing */
			case 'n':
				(void)va_arg(arglist, int);
				break;
			/* strings */
			case 's':
				tmp = va_arg(arglist, char *);
				bytes += strlen(tmp);
				break;
			default:
				goto Error;
			}
			i++;
		}
		/* end if('%'), current character is a regular one */
		else {
			bytes++;
		}
	}
	bytes++;
	va_end(arglist);

	if (dest) {
		tmp = (char *)realloc(dest, bytes);
	}
	else {
		tmp = (char *)calloc(bytes, sizeof(char));
	}

	if (!tmp) {
		return NULL;
	}
	dest = tmp;

	/* append to the format string */
	va_start(arglist, fmt);
	vsnprintf(&dest[destlen], bytes - destlen, fmt, arglist);
	va_end(arglist);

	/* NULL term */
	dest[bytes] = 0;

	return dest;
Error:
#ifndef NDEBUG
	fprintf(stderr, "Invalid format specifier\n");
#endif
	return NULL;
}

size_t strfmtlen_d(long long number)
{
	if (number == 0)
		return 1;
	
	size_t count = 0;

	/* sign */
	if (number < 0)
		count++;

	while (number != 0) {
		number /= 10;
		count++;
	}
	return count;
}

size_t strfmtlen_u(unsigned long long number)
{
	if (number == 0)
		return 1;
	
	size_t count = 0;

	while (number != 0) {
		number /= 10;
		count++;
	}
	return count;
}


size_t strfmtlen_o(unsigned long long number)
{
	if (number == 0)
		return 1;

	size_t count = 0;

	while (number != 0) {
		number >>= 3;
		count++;
	}
	return count; /* 0o omitted */
}

size_t strfmtlen_x(unsigned long long number)
{
	if (number == 0)
		return 1;

	size_t count = 0;

	while (number != 0) {
		number >>= 4;
		count++;
	}
	return count; /* 0x omitted */
}

#define STRFMTLEN_X(Specifier, Type) \
	size_t strfmtlen_##Specifier(Type number) \
	{ \
		clear_fmt_buf(); \
		snprintf(_InternalFmtBuf, BUF_LEN, "%" #Specifier, number); \
		return strlen(_InternalFmtBuf); \
	}

STRFMTLEN_X(f, float)
STRFMTLEN_X(lf, double)
STRFMTLEN_X(e, double)
STRFMTLEN_X(a, double)

int strcat_safe(char *destination, char *source)
{
	void *tmp = realloc(destination, strlen(destination) + strlen(source) + 1);
	if (!tmp) {
		return 0;
	}
	(void)strcat(destination, source);
	return 1;
}