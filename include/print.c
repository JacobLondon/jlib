#include "print.h"
#include "debug.h"
#include <stdarg.h>
#include <string.h>

int HERE_PASS = 1;

void print(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    for (; *fmt; fmt++) {
        switch (*fmt) {
        case 'd':
            printf("%d ", va_arg(ap, int));
            HERE(1);
            break;
        case 'f':
            printf("%lf ", va_arg(ap, double));
            HERE(2);
            break;
        case 'n':
            printf("\n");
            HERE(3);
            break;
        case 's':
            printf("%s ", va_arg(ap, char *));
            HERE(4);
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
