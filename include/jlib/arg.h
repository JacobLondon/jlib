#ifndef JLIB_ARG_H
#define JLIB_ARG_H

#include <stdbool.h>

#include "stringify.h"

/**
 * Determine whether an argument is in argv
 */
bool jlib_arg_check(int argc, char **argv, const char *arg);

/**
 * Get the argument after the target arg
 */
char *jlib_arg_get(int argc, char **argv, const char *arg);

#endif // JLIB_ARG_H