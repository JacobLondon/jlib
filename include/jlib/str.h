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

#define _STRINGIFY(x) #x
#define STRINGIFY(x) _STRINGIFY(x)

#endif /* JLIB_STR_H */