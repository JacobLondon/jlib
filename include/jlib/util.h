#ifndef JLIB_UTIL_H
#define JLIB_UTIL_H

#include <stddef.h>

/* Used from the Linux kernel. Tells GCC or Clang
 * to do optimizations on branching in bottlenecked
 * situations when testing for a condition.
 */
#define likely(x)   __builtin_expect((x), 1)
#define unlikely(x) __builtin_expect((x), 0)

void xswap(void *x, void *y);
void swap(void *x, void *y);
void *copy(void *buf, size_t size);

#endif /* JLIB_UTIL_H */
