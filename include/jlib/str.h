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
 * Always reallocs to guarantee sizing
 */
int strcat_safe(char *destination, char *source);

int streq(const char *str0, const char *str1);
#ifndef strdup
char *strdup(const char *str);
#endif /* strdup */
char *strndup(const char *str, size_t n);

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
char *strin(char *haystack, char *needles);

/**
 * Return 1 if str starts with sub, else 0
 */
int strstarts(char *str, char *sub);

/**
 * Return 1 if str ends with sub, else 0
 */
int strends(char *str, char *sub);

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

#endif /* JLIB_STR_H */
