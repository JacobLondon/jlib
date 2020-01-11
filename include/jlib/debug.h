#ifndef JLIB_DEBUG_H
#define JLIB_DEBUG_H

#include <stdio.h>

extern int _InternalHerePass;
#define HERE_OFF _InternalHerePass = 0
#define HERE_ON _InternalHerePas = 1

#define HERE(num) \
	if (_InternalHerePass) \
		fprintf(stderr, "%s:%s:%d: Here %d\n", __FILE__, __func__, __LINE__, num)

void halt(const char *message);

/* cpython's pymacro.h
   Assert a build-time dependency, as an expression.
   Your compile will fail if the condition isn't true, or can't be evaluated
   by the compiler. This can be used in an expression: its value is 0.

   Written by Rusty Russell, public domain, http://ccodearchive.net/ */

#define BUILD_ASSERT_EXPR(cond) \
	(sizeof(char [1 - 2*!(cond)]) - 1)

#define BUILD_ASSERT(cond) do { \
		(void)BUILD_ASSERT_EXPR(cond); \
	} while(0)

#endif /* JLIB_DEBUG_H */
