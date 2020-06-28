#ifndef JLIB_MALLOG_H
#define JLIB_MALLOG_H

#include <stddef.h>

#ifndef STRINGIFY
#define XSTRINGIFY(x) #x
#define STRINGIFY(x) XSTRINGIFY(x)
#endif

void *_log_malloc(size_t size, const char *sizeinfo, const char *file, const char *func, const char *line);
void *_log_calloc(size_t nmemb, size_t size, const char *sizeinfo, const char *file, const char *func, const char *line);
void *_log_realloc(void *ptr, size_t size, const char *sizeinfo, const char *file, const char *func, const char *line);
void *_log_strdup(const char *s, const char *sizeinfo, const char *file, const char *func, const char *line);
void _log_free(void *ptr, const char *file, const char *func, const char *line);

/**
 * malloc with detailed info, free with log_free
 */
#define log_malloc(size) _log_malloc(size, #size, __FILE__, __func__, STRINGIFY(__LINE__))

/**
 * calloc with detailed info, free with log_free
 */
#define log_calloc(nmemb, size) _log_calloc(nmemb, size, #size, __FILE__, __func__, STRINGIFY(__LINE__))

/**
 * realloc with detailed info, free with log_free
 */
#define log_realloc(ptr, size) _log_realloc(ptr, size, #size, __FILE__, __func__, __LINE__)

/**
 * strdup with detailed info, free with log_free
 */
#define log_strdup(str) _log_strdup(str, "String", __FILE__, __func__, STRINGIFY(__LINE__))

/**
 * free with detailed info
 */
#define log_free(ptr) _log_free(ptr, __FILE__, __func__, STRINGIFY(__LINE__))


/**
 * Initialize the log util. Will automatically get called
 * upon usage of any log functions
 */
void log_init(void);

/**
 * Clean up the log util, will not get automatically called
 */
void log_cleanup(void);

/**
 * View detailed printout of the log
 */
void log_dump(void);

#endif /* JLIB_MALLOG_H */
