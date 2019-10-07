#include <stdarg.h>
#include <string.h>
#include "print.h"

void print(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    for (; *fmt; fmt++) {
        switch (*fmt) {
        case 'd':
            printf("%d ", va_arg(ap, int));
            break;
        case 'f':
            printf("%lf ", va_arg(ap, double));
            break;
        case 'n':
            printf("\n");
            break;
        case 's':
            printf("%s ", va_arg(ap, char *));
            break;
        }
    }
    va_end(ap);
}

/**
 * test
 */

void print_test()
{
    print("sn", "test1");
    print("dsfn", 10, "test2", 1.10);
}
