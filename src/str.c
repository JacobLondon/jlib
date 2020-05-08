#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

#include <wchar.h>
#include <ctype.h>
#include <stdint.h>

#include <jlib/str.h>

/**
 * Only works with the following formats:
 * # means explicitly unsupported
 * 
 * %d   %i   %u   %o   %x   %X   %e   %E   %a   %A   %f   %F   %c   %n   %s   %p   %%
 * %ld  %li  %lu  %lo  %lx  %lX  %le  %lE  %la  %lA  %lf  %lF  #lc  %ln  %ls
 * %lld %lli %llu %llo %llx %llX                                    %lln
 * %zd  %zi  %zu  %zo  %zx  %zX                                     %zn
 * %jd  %ji  %ju  %jo  %jx  %jX                                     %jn
 * %td  %ti  %tu  %to  %tx  %tX                                     %tn
 * 
 */
int strcatf(char **buffer, const char *format, ...)
{
	char buf[350];
	long long int buffer_size;
	size_t format_size = strlen(format);
	va_list ap;
	long long int bytes;
	char *p, *tmp;

	enum X_TYPE_INFO {
		X_NONE = 0x00,
		X_L    = 0x01,
		X_LL   = 0x02,
		X_Z    = 0x04,
		X_J    = 0x08,
		X_T    = 0x10
	};
	unsigned char type_info;

	if (format == NULL || buffer == NULL) {
		return 0;
	}

	/* allow for NULL buffer, create a new string */
	if (*buffer != NULL) {
		buffer_size = strlen(*buffer);
	}
	else {
		buffer_size = 0;
	}

	/**
	 * There's a critical bug here, the format size needs to decrease by
	 * the amount of each format specifier, then increase in size by what
	 * fills it. This needs to be seperate to buffer/format size
	 * 
	 */

	va_start(ap, format);
	for (p = (char *)format, bytes = buffer_size; p != format + format_size; ++p) {
		if (*p != '%') continue;
		if (++p == format + format_size) break;
		
		for (tmp = buf; tmp != buf + sizeof(buf) && *tmp != '\0'; *tmp++ = '\0');
		type_info = X_NONE;
		again:
		switch (*p) {
			case '%': // special case, just go to entirely next item
				bytes++;
				continue;
			case 'l':
				if (type_info & X_L) type_info = X_LL;
				else type_info = X_L;
				goto next;
			case 'z':
				type_info = X_Z;
				goto next;
			case 'j':
				type_info = X_J;
				goto next;
			case 't':
				type_info = X_T;
			next:
				if (++p == format + format_size) goto done;
				goto again;
			default:
				break;
		}

		#define X_FORMAT(Fmt, Type) \
			snprintf(buf, sizeof(buf), (Fmt), va_arg(ap, Type)); \
			bytes -= (sizeof(Fmt) - 1); format_size -= (sizeof(Fmt) - 1); \
			printf("%" Fmt " %d, subbing %zu\n", __LINE__, sizeof(Fmt) - 1); break
		
		switch (*p) {
			case 'd': switch (type_info) {
				case X_NONE: X_FORMAT(  "%d", int);
				case X_L:    X_FORMAT( "%ld", long int);
				case X_LL:   X_FORMAT("%lld", long long int);
				case X_Z:    X_FORMAT( "%zd", size_t);
				case X_J:    X_FORMAT( "%jd", intmax_t);
				case X_T:    X_FORMAT( "%td", ptrdiff_t);
				default:     assert(0);
			}
			break;
			case 'i': switch (type_info) {
				case X_NONE: X_FORMAT(  "%i", int);
				case X_L:    X_FORMAT( "%li", long int);
				case X_LL:   X_FORMAT("%lli", long long int);
				case X_Z:    X_FORMAT( "%zi", size_t);
				case X_J:    X_FORMAT( "%ji", intmax_t);
				case X_T:    X_FORMAT( "%ti", ptrdiff_t);
				default:     assert(0);
			}
			break;
			case 'x': case 'X': switch (type_info) {
				case X_NONE: X_FORMAT(  "%x", unsigned int);
				case X_L:    X_FORMAT( "%lx", unsigned long int);
				case X_LL:   X_FORMAT("%llx", unsigned long long int);
				case X_Z:    X_FORMAT( "%zx", size_t);
				case X_J:    X_FORMAT( "%jx", uintmax_t);
				case X_T:    X_FORMAT( "%tx", ptrdiff_t);
				default:     assert(0);
			}
			break;
			case 'u': switch (type_info) {
				case X_NONE: X_FORMAT(  "%u", unsigned int);
				case X_L:    X_FORMAT( "%lu", unsigned long int);
				case X_LL:   X_FORMAT("%llu", unsigned long long int);
				case X_Z:    X_FORMAT( "%zu", size_t);
				case X_J:    X_FORMAT( "%ju", uintmax_t);
				case X_T:    X_FORMAT( "%tu", ptrdiff_t);
				default:     assert(0);
			}
			break;
			case 'e': case 'E': switch (type_info) {
				case X_NONE: X_FORMAT(  "%e", double);
				case X_L:    X_FORMAT( "%le", double);
				default:     assert(0);
			}
			break;
			case 'a': case 'A': switch (type_info) {
				case X_NONE: X_FORMAT(  "%a", double);
				case X_L:    X_FORMAT( "%la", double);
				default:     assert(0);
			}
			case 'f': case 'F': switch (type_info) {
				case X_NONE: X_FORMAT(  "%f", double);
				case X_L:    X_FORMAT( "%lf", double);
				default:     assert(0);
			}
			break;
			case 'c': switch (type_info) {
				case X_NONE: X_FORMAT(  "%c", int);
				case X_L:    X_FORMAT( "%lc", wint_t);
				default:     assert(0);
			}
			break;
			case 'n': switch (type_info) {
				case X_NONE: X_FORMAT(  "%n", int*);
				case X_L:    X_FORMAT( "%ln", long int*);
				case X_LL:   X_FORMAT("%lln", long long int*);
				case X_Z:    X_FORMAT( "%zn", size_t*);
				case X_J:    X_FORMAT( "%jn", intmax_t*);
				case X_T:    X_FORMAT( "%tn", ptrdiff_t*);
				default:     assert(0);
			}
			break;
			case 's': switch (type_info) {
				case X_NONE: X_FORMAT(  "%s", char*);
				case X_L:    X_FORMAT( "%ls", wchar_t*);
				default:     assert(0);
			}
			break;
			case 'p': switch (type_info) {
				case X_NONE: X_FORMAT(  "%p", void*);
				default:     assert(0);
			}
			break;
		}
		bytes += strlen(buf);
	}
done:
	va_end(ap);

	if (*buffer != NULL) {
		tmp = realloc(*buffer, bytes + 1);
		if (!tmp) return 0;
		tmp[bytes] = '\0';
		*buffer = tmp;
	}
	else {
		tmp = calloc(bytes + 1, sizeof(char));
		*buffer = tmp;
	}
	printf("Bytes: %lld\n", bytes);
	//buffer_size = strlen(tmp);
	va_start(ap, format);
	vsnprintf(&tmp[buffer_size], bytes - buffer_size, format, ap);
	va_end(ap);
	return 1;
	#undef X_FORMAT
}

#if 0
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
	size_t destlen;
	size_t fmtlen = strlen(fmt);
	va_list arglist;
	size_t i, bytes;
	char *tmp;

	va_start(arglist, fmt);

	if (!fmt) {
		return NULL;
	}

	/* allow for NULL dest, create a new string */
	if (dest) {
		destlen = strlen(dest);
	}
	else {
		destlen = 0;
	}

	/* record the number of bytes needed to hold the formatted string */
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
#endif

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
