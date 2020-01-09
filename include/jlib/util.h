#ifndef JLIB_UTIL_H
#define JLIB_UTIL_H

/* Used from the Linux kernel. Tells GCC or Clang
 * to do optimizations on branching in bottlenecked
 * situations when testing for a condition.
 * 
 * Note (Found on StackOverflow):
 * In general, you shouldn't be using __builtin_expect unless:
 * - You have a very real performance issue
 * - You've already optimized the algorithms in the system appropriately
 * - You've got performance data to back up your assertion that a particular case is the most likely
 */
#define likely(x)   __builtin_expect((x), 1)
#define unlikely(x) __builtin_expect((x), 0)

void jlib_swap(void *x, void *y);
void jlib_clear(void *a, int bytes);

#endif /* JLIB_UTIL_H */