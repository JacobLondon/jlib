#ifndef MYLIB_PRINT_H
#define MYLIB_PRINT_H

#include <stdio.h>

void print(const char *fmt, ...);

extern int HERE_PASS;
#define HERE_OFF HERE_PASS = 0
#define HERE_ON HERE_PASS = 1

#define HERE(num) \
    if (HERE_PASS) \
        fprintf(stderr, "%s:%s:%d: Here %d\n", __FILE__, __func__, __LINE__, num)

#endif // MYLIB_PRINT_H