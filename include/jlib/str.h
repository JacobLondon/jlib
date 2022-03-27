#ifndef JLIB_STR_H
#define JLIB_STR_H

#include <stddef.h>
#include <string.h>

/**
 * \brief Use printf modifiers to cat to a string with size reallocation.
 * Implementation only accepts specifiers, using flags, widths,
 * or precisions will exit with an error.
 * \param buffer Base address to string. String can be NULL, but the pointer may not be
 * \return 1 on success, 0 on failure
 * 
 * Usage:
 *   char *new = NULL;
 *   strcatf(&new, "%d", 100);
 *     or
 *   char *build = strdup("Lorem");
 *   strcatf(&build, "%s", "Ipsum");
 */
int strcatf(char **buffer, const char *format, ...);

/**
 * Like strcat but reallocs. Either destination/source can be NULL
 * Both NULL returns NULL
 */
char *strallocat(char *destination, char *source);

#define streq(str0, str1) (strcmp((str0), (str1)) == 0)
#define strcaseeq(str0, str1) (strcasecmp((str0), (str1)) == 0)

#ifndef strdup
char *strdup(const char *str);
#endif /* strdup */
char *strndup(const char *str, size_t n);

#ifndef strlcpy
/**
 * Slower than xstrlcpy as it needs to get the number of bytes written
 */
size_t strlcpy(char *dest, const char *src, size_t size);
#endif
char *xstrlcpy(char *dest, const char *src, size_t size);

/** 
 * Split a string by the fmt string.
 * All characters in the fmt string will not be included
 * in the result.
 * 
 * Return a NULL terminated array of strings
 */
char **strsplit(char *s, const char *fmt);
void strsplit_free(char **buf);

/**
 * Replace all instances of a substring from within a string
 */
int streplace(char **s, const char *old, const char *new);

/**
 * Similar to strstr, but returns the first pointer to any
 * of the needles in the haystack
 */
char *strchrany(char *haystack, char *needles);

/**
 * Return 1 if str starts with sub, else 0
 */

int strstarts(char *str, char *sub);
int strnstarts(char *str, char *sub, size_t sub_len);
#define STRNSTARTS(str, literal) strnstarts((str), literal, sizeof(literal) - 1)

int strcasestarts(char *str, char *sub);
int strncasestarts(char *str, char *sub, size_t sub_len);
#define STRNCASESTARTS(str, literal) (strncasecmp((str), literal, sizeof(literal) - 1) == 0)

/**
 * Return 1 if str ends with sub, else 0
 */
int strends(char *str, char *sub);

/**
 * like VIM's /search/e, return pointer at the end of the match if found or NULL
 */
char *strstre(char *haystack, char *needle);
char *strchre(char *haystack, char needle);

/**
 * Assuming *string isspace, move forward until there is no space, and get that char
 */
char *strchr_space(char *string); // find first space char
char *strpass_space(char *string); // pass space until first non-space char
char *strpass_alnum(char *string); // pass alnum until anything else
char *strpass_words(char *string, int words_to_pass);

/**
 * Replace multiples of spacing with a single space
 */
void strtrimspaces(char *string, char *out, size_t out_len);

/**
 * Return 0 if re has a match in str, else Regex error code.
 */
#if 0
int strstregex(char *str, regex_t re);
#endif

#ifndef STRINGIFY
#define XSTRINGIFY(x) #x
#define STRINGIFY(x) XSTRINGIFY(x)
#endif

/**
 * Non-null terminated strings, looking into another string
 */
struct sslice {
	const char *str;  /* NOT NULL TERMINATED */
	size_t len; /* length of view */
};

struct sslice sslice_new(const char *str, size_t len);
struct sslice sslice_cpy(struct sslice *self);
int sslice_cmp(struct sslice *self, struct sslice *other);
int sslice_scmp(struct sslice *self, char *str);
char *sslice_strchr(struct sslice *self, int c);
void sslice_put(struct sslice *self);
void sslice_fput(struct sslice *self, FILE *stream);
#if 0
struct token {
	size_t id;
	size_t idx;
	size_t lineno;
	size_t colno;
	char str[1]; /* cheeky trick, will store str with the same allocation as token
	                just need to make sure the string is on the same byte boundary
	                as size_t */
};

struct token *token_new(size_t id, const char *str, size_t idx, size_t lineno, size_t colno);
void token_free(struct token *self);
void token_put(struct token *self);

struct tokenizer {
	FILE *stream;
	struct token *(*gettok)(FILE *stream);
	struct token *cur[3];
};

struct tokenizer *tokenizer_new(const char *file, struct token *(*gettok)(FILE *stream));
void tokenizer_free(struct tokenizer *self);

/**
 * Clear the cur tokens array
 */
void tokenizer_clear(struct tokenizer *self);

/**
 * Get a shared pointer to a current token, the tokenizer controls the ownership
 * Multiple items need to be indexed to get the token pointer:
 * 
 * struct token *tok = tokenizer_get3(mytokernizer);
 * token_put(tok[0]);
 * token_put(tok[1]);
 * token_put(tok[2]);
 * 
 * If eof at any point, get back NULL
 */
struct token *tokenizer_get1(struct tokenizer *self);
struct token **tokenizer_get2(struct tokenizer *self);
struct token **tokenizer_get3(struct tokenizer *self);
#endif

#endif /* JLIB_STR_H */
