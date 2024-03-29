#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <jlib/str.h>

#include <wchar.h>
#include <ctype.h>
#include <stdint.h>

/**
 * Only works with the following formats:
 * 
 * %d   %i   %u   %o   %x   %X   %e   %E   %a   %A   %f   %F   %c   %n   %s   %p   %%
 * %ld  %li  %lu  %lo  %lx  %lX  %le  %lE  %la  %lA  %lf  %lF  %lc  %ln  %ls
 * %lld %lli %llu %llo %llx %llX                                    %lln
 * %zd  %zi  %zu  %zo  %zx  %zX                                     %zn
 * %jd  %ji  %ju  %jo  %jx  %jX                                     %jn
 * %td  %ti  %tu  %to  %tx  %tX                                     %tn
 * 
 * Sizing, time, colors, etc... are NOT supported
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

	enum {
		X_NONE = 0x00,
		X_L    = 0x01,
		X_LL   = 0x02,
		X_Z    = 0x04,
		X_J    = 0x08,
		X_T    = 0x10
	} type_info;

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

	va_start(ap, format);
	for (p = (char *)format, bytes = buffer_size + format_size; p != format + format_size; ++p) {
		if (*p != '%') continue;
		if (++p == format + format_size) break;
		
		for (tmp = buf; tmp != buf + sizeof(buf) && *tmp != '\0'; *tmp++ = '\0');
		type_info = X_NONE;
		again:
		switch (*p) {
			case '%': // special case, just go to entirely next item
				bytes--;
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
			bytes -= (sizeof(Fmt) - 1); break
		
		switch (*p) {
			case 'd': switch (type_info) {
				case X_NONE: X_FORMAT(  "%d", int);
				case X_L:    X_FORMAT( "%ld", long int);
				case X_LL:   X_FORMAT("%lld", long long int);
				case X_Z:    X_FORMAT( "%zd", size_t);
				case X_J:    X_FORMAT( "%jd", intmax_t);
				case X_T:    X_FORMAT( "%td", ptrdiff_t);
				default:     return 0;
			}
			break;
			case 'i': switch (type_info) {
				case X_NONE: X_FORMAT(  "%i", int);
				case X_L:    X_FORMAT( "%li", long int);
				case X_LL:   X_FORMAT("%lli", long long int);
				case X_Z:    X_FORMAT( "%zi", size_t);
				case X_J:    X_FORMAT( "%ji", intmax_t);
				case X_T:    X_FORMAT( "%ti", ptrdiff_t);
				default:     return 0;
			}
			break;
			case 'x': /* fallthrough */
			case 'X': switch (type_info) {
				case X_NONE: X_FORMAT(  "%x", unsigned int);
				case X_L:    X_FORMAT( "%lx", unsigned long int);
				case X_LL:   X_FORMAT("%llx", unsigned long long int);
				case X_Z:    X_FORMAT( "%zx", size_t);
				case X_J:    X_FORMAT( "%jx", uintmax_t);
				case X_T:    X_FORMAT( "%tx", ptrdiff_t);
				default:     return 0;
			}
			break;
			case 'u': switch (type_info) {
				case X_NONE: X_FORMAT(  "%u", unsigned int);
				case X_L:    X_FORMAT( "%lu", unsigned long int);
				case X_LL:   X_FORMAT("%llu", unsigned long long int);
				case X_Z:    X_FORMAT( "%zu", size_t);
				case X_J:    X_FORMAT( "%ju", uintmax_t);
				case X_T:    X_FORMAT( "%tu", ptrdiff_t);
				default:     return 0;
			}
			break;
			case 'e': /* fallthrough */
			case 'E': switch (type_info) {
				case X_NONE: X_FORMAT(  "%e", double);
				case X_L:    X_FORMAT( "%le", double);
				default:     return 0;
			}
			break;
			case 'a': /* fallthrough */
			case 'A': switch (type_info) {
				case X_NONE: X_FORMAT(  "%a", double);
				case X_L:    X_FORMAT( "%la", double);
				default:     return 0;
			}
			break;
			case 'f': /* fallthrough */
			case 'F': switch (type_info) {
				case X_NONE: X_FORMAT(  "%f", double);
				case X_L:    X_FORMAT( "%lf", double);
				default:     return 0;
			}
			break;
			case 'c': switch (type_info) {
				case X_NONE: X_FORMAT(  "%c", int);
				case X_L:    X_FORMAT( "%lc", wint_t);
				default:     return 0;
			}
			break;
			case 'n': switch (type_info) {
				case X_NONE: X_FORMAT(  "%n", int*);
				case X_L:    X_FORMAT( "%ln", long int*);
				case X_LL:   X_FORMAT("%lln", long long int*);
				case X_Z:    X_FORMAT( "%zn", size_t*);
				case X_J:    X_FORMAT( "%jn", intmax_t*);
				case X_T:    X_FORMAT( "%tn", ptrdiff_t*);
				default:     return 0;
			}
			break;
			case 's': switch (type_info) {
				case X_NONE:
					tmp = va_arg(ap, char*);
					assert(tmp);
					bytes -= sizeof("%s") - 1;
					bytes += strlen(tmp);
					continue;
				case X_L:
					tmp = (char *)va_arg(ap, wchar_t*);
					assert(tmp);
					bytes -= sizeof("%ls") - 1;
					bytes += wcslen((wchar_t *)tmp) * sizeof(wchar_t);
					continue;
				default:     assert(0);
			}
			break;
			case 'p': switch (type_info) {
				case X_NONE: X_FORMAT(  "%p", void*);
				default:     return 0;
			}
			break;
		}
		bytes += strlen(buf);
	}
done:
	va_end(ap);
	bytes++; /* NUL term */

	if (*buffer != NULL) {
		tmp = realloc(*buffer, bytes + 1);
		if (!tmp) return 0;
		tmp[bytes] = '\0';
		*buffer = tmp;
	}
	else {
		tmp = calloc(bytes + 1, sizeof(char));
		if (!tmp) return 0;
		*buffer = tmp;
	}
	va_start(ap, format);
	vsnprintf(&tmp[buffer_size], bytes - buffer_size, format, ap);
	va_end(ap);
	return 1;
	#undef X_FORMAT
}

char *strallocat(char *destination, char *source)
{
	if (!destination && !source) {
		return NULL;
	}
	size_t size = 0;
	size += destination ? strlen(destination) : 0;
	size += source ? strlen(source) : 0;

	void *tmp;
	if (!destination) {
		tmp = malloc(size + 1);
	}
	else {
		tmp = realloc(destination, size + 1);
	}

	if (!tmp) {
		return NULL;
	}

	destination = tmp;
	(void)strcat(destination, source);
	destination[size] = '\0';
	return destination;
}

#ifndef strdup
char *strdup(const char *str)
{
	size_t len = strlen(str);
	char *buf = malloc(len + 1);
	if (!buf) return NULL;
	(void)memcpy(buf, str, len + 1); // copy NUL
	return buf;
}
#endif /* strdup */

char *strndup(const char *str, size_t n)
{
	size_t size = strlen(str);
	size = size > n ? n : size;
	char *buf = malloc(size + 1);
	if (!buf) return NULL;
	(void)memcpy(buf, str, size);
	buf[size] = 0; // manual NUL
	return buf;
}

#ifndef strlcpy
size_t strlcpy(char *dest, const char *src, size_t size)
{
	size_t len = strlen(src);
	strncpy(dest, src, size);
	if (size > 0)
		dest[size - 1]= '\0';
	return (len > size) ? size : len;
}
#endif

char *xstrlcpy(char *dest, const char *src, size_t size)
{
	strncpy(dest, src, size);
	if (size > 0)
		dest[size - 1]= '\0';
	return dest;
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
		buf[i] = NULL;
	}
	free(buf);
}

int streplace(char **s, const char *old, const char *new)
{
	char *result;
	char *sub;
	char *p;
	size_t len;
	size_t old_len;
	size_t new_len;

	if (s == NULL || *s == NULL || old == NULL || new == NULL) {
		return 0;
	}

	p = *s;
	len = strlen(*s);
	old_len = strlen(old);
	new_len = strlen(new);

	/* no replacements found */
	if ((sub = strstr(p, old)) == NULL) {
		return 0;
	}

	/* at least one replacement found */
	do {
		len = len - old_len + new_len;
		p = (char *)((size_t)sub + old_len);
	} while ((sub = strstr(p, old)));
	
	result = calloc(len + 1, sizeof(char));
	if (!result) {
		return 0;
	}
	
	/* re-add the bytes back into the result */
	for (p = *s; *p != '\0'; p++) {
		if (p == strstr(p, old)) {
			strcat(result, new);
			p += old_len - 1;
		}
		else {
			strncat(result, p, 1);
		}
	}

	free(*s);
	*s = result;

	return 1;
}

char *strchrany(char *haystack, char *needles)
{
	char *p, *ret;
	
	assert(haystack);
	if (!needles) {
		return NULL;
	}

	for (p = needles; *p != '\0'; p++) {
		ret = strchr(haystack, *p);
		if (ret) {
			return ret;
		}
	}
	return NULL;
}

int strstarts(char *str, char *sub)
{
	size_t str_len = strlen(str);
	size_t sub_len = strlen(sub);
	str_len = (str_len > sub_len) ? sub_len : str_len;
	return (strncmp(str, sub, str_len) == 0);
}

int strnstarts(char *str, char *sub, size_t sub_len)
{
	size_t str_len = strlen(str);
	str_len = (str_len > sub_len) ? sub_len : str_len;
	return (strncmp(str, sub, str_len) == 0);
}

int strcasestarts(char *str, char *sub)
{
	size_t str_len = strlen(str);
	size_t sub_len = strlen(sub);
	str_len = (str_len > sub_len) ? sub_len : str_len;
	return (strcasecmp(str, sub) == 0);
}

int strncasestarts(char *str, char *sub, size_t sub_len)
{
	size_t str_len = strlen(str);
	str_len = (str_len > sub_len) ? sub_len : str_len;
	return (strncasecmp(str, sub, str_len) == 0);
}

int strends(char *str, char *sub)
{
	assert(str);
	if (!sub) {
		return 0;
	}

	if (strstr(str, sub) == (char *)(strlen(str) - strlen(sub))) {
		return 1;
	}
	return 0;
}

char *strstre(char *haystack, char *needle)
{
	if (!haystack || !needle) {
		return NULL;
	}

	char *p = strstr(haystack, needle);
	if (!p) {
		return NULL;
	}

	return &p[strlen(needle)];
}

char *strchre(char *haystack, char needle)
{
	if (!haystack) {
		return NULL;
	}

	char *p = strchr(haystack, needle);
	if (!p) {
		return NULL;
	}

	return &p[1];
}

char *strchr_space(char *string)
{
	if (!string) {
		return NULL;
	}

	while (*string && !isspace(*string)) {
		string++;
	}
	return string;
}

char *strpass_space(char *string)
{
	if (!string) {
		return NULL;
	}

	while (isspace(*string)) {
		string++;
	}

	return string;
}

char *strpass_alnum(char *string)
{
	if (!string) {
		return NULL;
	}

	while (isalnum(*string)) {
		string++;
	}

	return string;
}

char *strpass_words(char *string, int words_to_pass)
{
	if (!string) {
		return NULL;
	}

	while (string && (words_to_pass > 0)) {
		string = strpass_space(string);
		string = strchr_space(string);
	}
	string = strpass_space(string);
	return string;
}

void strtrimspaces(char *string, char *out, size_t out_len)
{
	if (!string || !out) {
		return;
	}

	int inspace = 0;
	size_t i, j;
	for (i = 0, j = 0; string[i] && (j < out_len); i++) {
		if (isspace(string[i])) {
			if (inspace == 0) {
				out[j] = ' ';
				j++;
				inspace = 1;
			}
		}
		else {
			out[j] = string[i];
			j++;
			inspace = 0;
		}
	}
}

#if 0
int strstregex(char *str, regex_t re)
{
	int ret;
	char err[128];
	assert(str);
	ret = regexec(&re, str, 0, NULL, 0);
	if (!ret) {
		return 0;
	}
	else if (ret == REG_NOMATCH) {
		return ret;
	}
	else {
		regerror(ret, &re, err, sizeof(err));
		fprintf(stderr, "Regex Failure: %s\n", err);
		assert(0);
	}
	return 1;
}
#endif

struct sslice sslice_new(const char *str, size_t len)
{
	return (struct sslice){ str, len };
}

struct sslice sslice_cpy(struct sslice *self)
{
	return (struct sslice){ self->str, self->len };
}

int sslice_cmp(struct sslice *self, struct sslice *other)
{
	return strncmp(self->str, other->str, self->len);
}

int sslice_scmp(struct sslice *self, char *str)
{
	return strncmp(self->str, str, self->len);
}

char *sslice_strchr(struct sslice *self, int c)
{
	return memchr(self->str, c, self->len);
}

void sslice_put(struct sslice *self)
{
	sslice_fput(self, stdout);
}

void sslice_fput(struct sslice *self, FILE *stream)
{
	char *p = (char *)self->str;
	for (; (size_t)p - (size_t)self->str < (size_t)self->len; p++) {
		(void)putc(*p, stream);
	}
}

#if 0
struct token *token_new(size_t id, const char *str, size_t idx, size_t lineno, size_t colno)
{
	/* store the base token info, and the string in the same allocation */
	size_t len = strlen(str);
	struct token *self = calloc(
		3 + (size_t)ceil((double)len / (double)sizeof(size_t)),
		sizeof(size_t));
	assert(self);
	self->id = id;
	self->idx = idx;
	self->lineno = lineno;
	self->colno = colno;
	memcpy(self->str, str, len);
	return self;
}

void token_free(struct token *self)
{
	assert(self);
	free(self);
}


void token_put(struct token *self)
{
	assert(self);
	printf("%zu(%s):%zu:%zu\n", self->id, self->str, self->lineno, self->colno);
}

struct tokenizer *tokenizer_new(const char *file, struct token *(*gettok)(FILE *stream))
{
	struct tokenizer *self = calloc(1, sizeof(struct tokenizer));
	assert(self);
	self->stream = fopen(file, "r");
	assert(self->stream);
	self->gettok = gettok;
	return self;
}

void tokenizer_free(struct tokenizer *self)
{
	assert(self);
	if (self->stream) {
		fclose(self->stream);
	}
	tokenizer_clear(self);
	free(self);
}

void tokenizer_clear(struct tokenizer *self)
{
	assert(self);

	if (self->cur[0]) {
		token_free(self->cur[0]);
		self->cur[0] = NULL;
	}
	if (self->cur[1]) {
		token_free(self->cur[1]);
		self->cur[0] = NULL;
	}
	if (self->cur[2]) {
		token_free(self->cur[2]);
		self->cur[0] = NULL;
	}
}

struct token *tokenizer_get1(struct tokenizer *self)
{
	tokenizer_clear(self);

	if (feof(self->stream)) {
		return NULL;
	}

	self->cur[0] = self->gettok(self->stream);
	return self->cur[0];
}

struct token **tokenizer_get2(struct tokenizer *self)
{
	tokenizer_clear(self);

	if (feof(self->stream)) {
		return NULL;
	}

	self->cur[0] = self->gettok(self->stream);

	if (feof(self->stream)) {
		return NULL;
	}

	self->cur[1] = self->gettok(self->stream);

	return self->cur;
}

struct token **tokenizer_get3(struct tokenizer *self)
{
	tokenizer_clear(self);

	if (feof(self->stream)) {
		return NULL;
	}

	self->cur[0] = self->gettok(self->stream);

	if (feof(self->stream)) {
		return NULL;
	}

	self->cur[1] = self->gettok(self->stream);

	if (feof(self->stream)) {
		return NULL;
	}

	self->cur[2] = self->gettok(self->stream);

	return self->cur;
}
#endif
