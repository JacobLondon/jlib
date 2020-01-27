#ifndef JLIB_DEBUG_H
#define JLIB_DEBUG_H

#include <stdio.h>

extern int _InternalHerePass;
#define HERE_OFF _InternalHerePass = 0
#define HERE_ON _InternalHerePass = 1

#ifndef _STRINGIFY
	#define _STRINGIFY(x) #x
	#ifndef STRINGIFY
		#define STRINGIFY(x) _STRINGIFY(x)
	#endif
#endif /* STRINGIFY */

#define HERE(num) \
	if (_InternalHerePass) \
		fprintf(stderr, "%s:%s:%s: Here %d\n", __FILE__, __func__, STRINGIFY(__LINE__), num)

void halt(const char *message);


/* cpython's pymacro.h
   Assert a build-time dependency, as an expression.
   Your compile will fail if the condition isn't true, or can't be evaluated
   by the compiler. This can be used in an expression: its value is 0.

   Written by Rusty Russell, public domain, http://ccodearchive.net/ */

#if !defined(static_assert)
	#ifdef NDEBUG
		#define static_assert(cond) ((void)0)
	#else
		#define static_assert_expr(cond) \
			(sizeof(char [1 - 2*!(cond)]) - 1)

		#define static_assert(cond) do { \
				(void)static_assert_expr(cond); \
			} while(0)

	#endif /* NDEBUG */
#endif

#define uassert(expression) \
	do { \
		if (!(expression)) { \
			fprintf(stderr, "%s:%s:%s: Assertion failure: '"#expression "'\n", \
				__FILE__, __func__, STRINGIFY(__LINE__)); \
			exit(-1); \
		} \
	} while (0)

#endif /* JLIB_DEBUG_H */
