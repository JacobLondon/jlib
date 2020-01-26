#include <stdarg.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

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
	union {
		int i;
		unsigned u;
		long l;
		long long ll;
		float f;
		double d;
		long double ld;
		size_t s;
	} arg;

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
					case 'i':
						bytes += strfmtlen_d(va_arg(arglist, int));
						break;
					case 'u':
						bytes += strfmtlen_u(va_arg(arglist, unsigned));
						break;
					case 'o':
						bytes += strfmtlen_o(va_arg(arglist, unsigned));
						break;
					case 'x':
					case 'X':
						bytes += strfmtlen_x(va_arg(arglist, int));
						break;
					default:
						goto Error;
					}
					i++;
					break;
				}
				/* short int: h[diuoxX] */
				case 'd':
				case 'i':
					bytes += strfmtlen_d(va_arg(arglist, int));
					break;
				case 'u':
					bytes += strfmtlen_u(va_arg(arglist, unsigned));
					break;
				case 'o':
					bytes += strfmtlen_o(va_arg(arglist, unsigned));
					break;
				case 'x':
				case 'X':
					bytes += strfmtlen_x(va_arg(arglist, int));
					break;
				default:
					goto Error;
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
					case 'i':
						bytes += strfmtlen_d(va_arg(arglist, long long));
						break;
					case 'u':
						bytes += strfmtlen_u(va_arg(arglist, unsigned long long));
						break;
					case 'o':
						bytes += strfmtlen_o(va_arg(arglist, unsigned long long));
						break;
					case 'x':
					case 'X':
						bytes += strfmtlen_x(va_arg(arglist, unsigned long long));
						break;
					default:
						goto Error;
					}
					i++;
					break;
				}
				/* double: lf */
				case 'f': case 'F':
					bytes += strfmtlen_lf(va_arg(arglist, double));
					i++;
					break;
				/* long int: l[diuoxX] */
				case 'd': /* TODO: Complete formatting */
				case 'i':
				case 'u':
				case 'o':
				case 'x':
				case 'X':
					(void)va_arg(arglist, long); i++;
					break;
				default:
					goto Error;
				}
				i++;
				break;
			} /* end long */

			/* floats */
			case 'f': case 'F': case 'e': case 'E':
			case 'g': case 'G': case 'a': case 'A':
				(void)va_arg(arglist, double);
				i++;
				break;
			/* long double */
			case 'L':
				(void)va_arg(arglist, long double);
				i++;
				break;
			/* int */
			case 'd': case 'i':
				(void)va_arg(arglist, int);
				i++;
				break;
			/* unsigned int */
			case 'u': case 'o': case 'x': case 'X':
				(void)va_arg(arglist, unsigned);
				i++;
				break;
			case 'p': /* pointer */
			case 'j': /* intmax_t or uintmax_t */
			case 'z': /* size_t */
			case 't': /* ptrdiff_t */
				(void)va_arg(arglist, size_t);
				i++;
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
			Success:
				break;
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

size_t strfmtlen_d(long long number)
{
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
	size_t count = 0;

	while (number != 0) {
		number /= 10;
		count++;
	}
	return count;
}


size_t strfmtlen_o(unsigned long long number)
{
	size_t count = 0;

	while (number != 0) {
		number >>= 3;
		count++;
	}
	return count; /* 0o omitted */
}

size_t strfmtlen_x(unsigned long long number)
{
	size_t count = 0;

	while (number != 0) {
		number >>= 4;
		count++;
	}
	return count; /* 0x omitted */
}

size_t strfmtlen_f(float number)
{
	/* Sign (1) Exponent (8) Mantissa (23) */

	union {
		float f;
		uint32_t i;
	} b;
	b.f = number;

	int sign = (b.i >> (32 - 1)); /* msb */
	int exponent = (b.i >> (32 - 9)) & 0xFFU;
	const int EXPONENT_BIAS = 0x7F;
	/* uint32_t mantissa = (b.i & 0x7FFFFFU); */

	size_t count = 0;

	/* '-' */
	if (sign)
		count++;
	
	/* infinity or NaN: 'inf' */
	if (exponent == 0xFF) {
		count += 3;
		return count;
	}
	/* zero or subnormal: '0' */
	else if (exponent == 0) {
		count += 1;
	}
	/* digits left of decimal */
	else {
		count += (size_t)log2f((float)(exponent - EXPONENT_BIAS)) + 1;
	}

	/* fractional part: '.XXXXXX' */
	return count + 1 + 6; /* 6 right of decimal by default */
}

size_t strfmtlen_lf(double number)
{
	/* Sign (1) Exponent (11) Mantissa (52) */

	union {
		double f;
		uint64_t i;
	} b;
	b.f = number;

	int sign = (b.i >> (64 - 1)); /* msb */
	int exponent = (b.i >> (64 - 12)) & 0x7FFU;
	const int EXPONENT_BIAS = 0x3FF;
	/* uint64_t mantissa = (b.i & 0xFFFFFFFFFFFFFULL); */

	size_t count = 0;

	/* '-' */
	if (sign)
		count++;
	
	/* infinity or NaN: 'inf' */
	if (exponent == 0x7FF) {
		count += 3;
		return count;
	}
	/* zero or subnormal: '0' */
	else if (exponent == 0) {
		count += 1;
	}
	/* digits left of decimal */
	else {
		count += (size_t)log2f((float)(exponent - EXPONENT_BIAS)) + 1;
	}

	/* fractional part (default len): '.XXXXXX' */
	return count + 1  /*.*/
				 + 6; /*XXXXXX */
}

size_t strfmtlen_e(double number)
{
	/* default format X.XXXXXXe[+-] */
	size_t count = 1 /*X*/
				 + 1 /*.*/
				 + 6 /*XXXXXX*/
				 + 1 /*e*/
				 + 1 /*+-*/;
	const int EXPONENT_BIAS = 0x3FF;
	int exponent = (((*(uint64_t *)&number) >> (64 - 12)) & 0x7FFU) - EXPONENT_BIAS;

	/* less than |100| always has 2 digits */
	if ((exponent < 100) && (exponent > -100))
		count += 2;
	/* count digits */
	else while (exponent != 0) {
		count++;
		exponent /= 10;
	}
	return count;
}

size_t strfmtlen_a(double number)
{
	/* always a double for this format... */
	union {
		double f;
		uint64_t i;
	} b;
	b.f = number;

	int sign = (b.i >> (64 - 1)); /* msb */
	const int EXPONENT_BIAS = 0x3FF;
	int exponent = ((b.i >> (64 - 12)) & 0x7FFU) - EXPONENT_BIAS;

	/* X.XXXXXXXXXXXXXp[+-] */
	size_t count = 1  /*X*/
				 + 1  /*.*/
				 + 13 /*XXXXXXXXXXXXX*/
				 + 1  /*p*/
				 + 1; /*+-*/

	if (sign)
		count++;

	/* count exponent size */
	while (exponent != 0) {
		exponent /= 10;
		count++;
	}
	return count;
}

void strcat_safe(char *destination, char *source)
{
	void *tmp = realloc(destination, strlen(destination) + strlen(source) + 1);
	if (!tmp) {
		fputs("Error: Could not realloc in strcat_safe", stderr);
		exit(-1);
	}
	strcat(destination, source);
}