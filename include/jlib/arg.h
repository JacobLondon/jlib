#ifndef JLIB_ARG_H
#define JLIB_ARG_H

/**
 * Determine whether an argument is in argv
 * 
 * @param argc Arg count
 * @param argv Arg vector
 * @param da
 *      A nullable string. If of the form "-X" then 'X' will be searched for in
 *      single letter groupings like -abpoXb
 * @param ddarg
 *      A nullable string to search for
 * @return
 *      0 if not found (in @a da or @a ddarg is argv[0], then it will still return 0)
 *      The index of either @a da or @a ddarg in @a argv if found
 */
int arg_check(int argc, char **argv, const char *da, const char *ddarg);

/**
 * Get the argument after the target arg
 * 
 * @param argc Arg count
 * @param argv Arg vector
 * @param da
 *      A nullable string. If of the form "-X" then 'X' will be searched for in
 *      single letter groupings like -abpoXb SOME_VALUE
 * @param ddarg
 *      A nullable string to search for
 * @return
 *      NULL if not found
 *      The string after @a da or @a ddarg if it is present
 */
char *arg_get(int argc, char **argv, const char *da, const char *ddarg);

#endif /* JLIB_ARG_H */