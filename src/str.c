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

union dbl {
    double f;
    uint64_t i;
    struct {
        uint64_t mantissa : 52;
        uint64_t exponent : 11;
        uint64_t sign : 1;
    } fields;
};

union flt {
	float f;
	uint32_t i;
	struct {
		uint32_t mantissa : 23;
		uint32_t exponent : 8;
		uint64_t sign : 1;
	} fields;
};

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
	/* Sign (1) Exponent (8) Mantissa (23) */

	union flt b;
	b.f = number;
	/* const int EXPONENT_BIAS = 0x7F; */

	size_t count = 0;

	/* '-' */
	if (b.fields.sign)
		count++;
	
	/* infinity or NaN: 'inf' */
	if (b.fields.exponent == 0xFF) {
		count += 3;
		return count;
	}
	/* zero or subnormal: '0' */
	else if (b.fields.exponent == 0) {
		count += 1;
	}
	/* digits left of decimal */
	else while (floor(number) != 0) {
		number /= 10.0f;
		count++;
	}

	/* fractional part: '.XXXXXX' */
	return count + 1 + 6; /* 6 right of decimal by default */
}

size_t strfmtlen_lf(double number)
{
	/* Sign (1) Exponent (11) Mantissa (52) */

	union dbl b;
	b.f = number;

	/* const int EXPONENT_BIAS = 0x3FF; */

	size_t count = 0;

	/* '-' */
	if (b.fields.sign)
		count++;
	
	/* infinity or NaN: 'inf' */
	if (b.fields.exponent == 0x7FF) {
		count += 3;
		return count;
	}
	/* zero or subnormal: '0' */
	else if (b.fields.exponent == 0) {
		count += 1;
	}
	/* digits left of decimal */
	else while (floor(number) != 0) {
		number /= 10.0;
		count++;
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
	
	union dbl b;
	b.f = number;

	const int EXPONENT_BIAS = 0x3FF;
	b.fields.exponent -= EXPONENT_BIAS;

	if (b.fields.sign)
		count++;

	printf("Exponent: %d\n", b.fields.exponent);
	/* less than |100| always has 2 digits */
	if ((b.fields.exponent < 100) && (b.fields.exponent > -100)) {
		count += 2;
	}
	/* count digits */
	else while (b.fields.exponent != 0) {
		b.fields.exponent /= 10;
		count++;
	}
	return count;
}

size_t strfmtlen_a(double number)
{
	/* always a double for this format... */
	union dbl b;
	b.f = number;

	const int EXPONENT_BIAS = 0x3FF;

	/* X.XXXXXXXXXXXXXp[+-] */
	size_t count = 1  /*X*/
				 + 1  /*.*/
				 + 13 /*XXXXXXXXXXXXX*/
				 + 1  /*p*/
				 + 1; /*+-*/

	if (b.fields.sign)
		count++;

	/* count exponent size */
	while (b.fields.exponent != 0) {
		b.fields.exponent /= 10;
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