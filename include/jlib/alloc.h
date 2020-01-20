#ifndef JLIB_ALLOC_H
#define JLIB_ALLOC_H

#include <stdlib.h>

#ifndef _STRINGIFY && STRINGIFY
#define _STRINGIFY(x) #x
#define STRINGIFY(x) _STRINGIFY(x)
#endif /* STRINGIFY */

#define try_malloc(Dest, Size) \
	do { \
		if (!((Dest) = malloc((Size)))) { \
			fprintf(stderr, "%s:%s:%s: Failed to malloc\n", __FILE__, __func__, STRINGIFY(__LINE__)); \
			exit(-1); \
		} \
	} while (0)

#define try_calloc(Dest, Count, Size) \
	do { \
		if (!((Dest) = calloc((Count), (Size)))) { \
			fprintf(stderr, "%s:%s:%s: Failed to calloc\n", __FILE__, __func__, STRINGIFY(__LINE__)); \
			exit(-1); \
		} \
	} while (0)

#define try_realloc(Addr, Size) \
	do { \
		void *_try_realloc_internal = realloc((Addr), (Size)); \
		if (!_try_realloc_internal) { \
			fprintf(stderr, "%s:%s:%s: Failed to realloc\n", __FILE__, __func__, STRINGIFY(__LINE__)); \
			exit(-1); \
		} else \
			(Addr) = _try_realloc_internal; \
	} while (0)

#define JLIB_ALLOC_H