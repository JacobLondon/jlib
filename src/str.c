#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include <jlib/str.h>

/* DBL_MAX's length = 316 characters, go to 512 for good measure */
enum { STRCATF_BUFSIZ = 512 };
static char strcatf_buf[STRCATF_BUFSIZ];
static void strcatf_clear_buf()
{
	int i = 0;
	for (; i < STRCATF_BUFSIZ; i++) {
		/* unlikely */
		if (__builtin_expect(strcatf_buf[i] == 0, 0)) {
			break;
		}
		else {
			strcatf_buf[i] = 0;
		}
	}
}

char *strcatf(char *dest, const char *fmt, ...)
{
	if (!fmt) {
		return NULL;
	}

	size_t destlen;
	size_t fmtlen = strlen(fmt);
	va_list arglist;
	va_start(arglist, fmt);

	/* allow for NULL dest, create a new string */
	if (dest) {
		destlen = strlen(dest);
	}
	else {
		destlen = 0;
	}

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

size_t strfmtlen_f(float number)
{
	strcatf_clear_buf();
	snprintf(strcatf_buf, STRCATF_BUFSIZ, "%f", number);
	return strlen(strcatf_buf);
}

size_t strfmtlen_lf(double number)
{
	strcatf_clear_buf();
	snprintf(strcatf_buf, STRCATF_BUFSIZ, "%lf", number);
	return strlen(strcatf_buf);
}

size_t strfmtlen_e(double number)
{
	strcatf_clear_buf();
	snprintf(strcatf_buf, STRCATF_BUFSIZ, "%e", number);
	return strlen(strcatf_buf);
}

size_t strfmtlen_a(double number)
{
	strcatf_clear_buf();
	snprintf(strcatf_buf, STRCATF_BUFSIZ, "%a", number);
	return strlen(strcatf_buf);
}

int strcat_safe(char *destination, char *source)
{
	void *tmp = realloc(destination, strlen(destination) + strlen(source) + 1);
	if (!tmp) {
		return 0;
	}
	(void)strcat(destination, source);
	return 1;
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
char *strdup(const char *str)
{
	size_t i;
	char *buf = calloc(strlen(str) + 1, sizeof(char));
	if (!buf) {
		return NULL;
	}
	for (i = 0; (buf[i] = str[i]); i++)
		;
	
	return buf;
}
#endif /* strdup */

char *strndup(const char *str, size_t n)
{
	size_t i;
	size_t size = strlen(str);
	size = size > n ? n : size;
	char *buf = calloc(size + 1, sizeof(char));
	if (!buf) {
		return NULL;
	}
	for (i = 0; i < size; i++) {
		buf[i] = str[i];
	}
	return buf;
}

char **strsplit(char *s, const char *fmt)
{
	if (!s || !fmt) {
		return NULL;
	}

	size_t word_count = 0;
	size_t size = strlen(s);
	char **buf;
	char *tmp;
	size_t i, j;

	/* count words */
	for (i = 0; i < size - 1; i++) {
		/* skip any invalid characters */
		if (strchr(fmt, s[i])) {
			continue;
		}

		/* word was found, traverse passed it */
		for (; !strchr(fmt, s[i]) && s[i]; i++)
			;
		word_count++;
	}

	/* hold each string and NULL terminate */
	buf = calloc(word_count + 1, sizeof(char *));
	if (!buf) {
		return NULL;
	}

	/* dup each string */
	for (i = 0, j = 0; i < word_count; i++) {
		/* probe forward for length */
		for (; j < size; j++) {
			if (strchr(fmt, s[j])) {
				continue;
			}
			
			/* word was found, traverse passed it */
			for (tmp = &s[j]; !strchr(fmt, s[j]) && s[j]; j++)
				;

			/* buf[i] might be NULL, no guarantees */
			buf[i] = strndup(tmp, (size_t)(&s[j] - tmp));
			break;
		}
	}
	return buf;
}

void strsplit_free(char **buf)
{
	size_t i = 0;
	for (; buf[i]; i++) {
		free(buf[i]);
	}
	free(buf);
}
