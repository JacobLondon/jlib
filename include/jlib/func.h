#ifndef JLIB_FUNC_H
#define JLIB_FUNC_H

#include <stddef.h>

/**
 * 1 if any items in the array, each passed by reference into holds_true returning a non-zero value
 */
int any(void *array, size_t size, size_t step, int (*holds_true)(void *value));

/**
 * 1 if all items in the array, each passed by reference into holds_true returning a non-zero value
 */
int all(void *array, size_t size, size_t step, int (*holds_true)(void *value));

#endif /* JLIB_FUNC_H */
