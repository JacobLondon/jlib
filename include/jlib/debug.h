#ifndef JLIB_DEBUG_H
#define JLIB_DEBUG_H

#include <stdio.h>
#include <assert.h>

#ifndef _STRINGIFY
	#define _STRINGIFY(x) #x
	#ifndef STRINGIFY
		#define STRINGIFY(x) _STRINGIFY(x)
	#endif
#endif /* STRINGIFY */

#define HERE(num) \
	fprintf(stderr, "%s:%s:%s: Here %d\n", __FILE__, __func__, STRINGIFY(__LINE__), num)

void halt(const char *message);


/* cpython's pymacro.h
   Assert a build-time dependency, as an expression.
   Your compile will fail if the condition isn't true, or can't be evaluated
   by the compiler. This can be used in an expression: its value is 0.

   Written by Rusty Russell, public domain, http://ccodearchive.net/ */

#if !defined(static_assert)
	#ifdef NDEBUG
		#define static_assert(cond, stmt) ((void)0)
	#else
		#define static_assert_expr(cond, stmt) \
			((void)stmt), (sizeof(char [1 - 2*!(cond)]) - 1)

		#define static_assert(cond, stmt) do { \
				(void)static_assert_expr(cond, stmt); \
			} while(0)

	#endif /* NDEBUG */
#endif

#if !defined(assert)
	#ifdef NDEBUG
		#define assert(cond) ((void)0)
	#else
		#define assert(cond) \
			do { \
				if (!(expression)) { \
					fprintf(stderr, "%s:%s:%s: Assertion failure: '"#cond "'\n", \
						__FILE__, __func__, STRINGIFY(__LINE__)); \
					exit(1); \
				} \
			} while (0)
	#endif /* NDEBUG */
#endif

#endif /* JLIB_DEBUG_H */
