#include <stdarg.h>
#include <string.h>
#include "print.h"

void print(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    printf(fmt, ap);
    va_end(ap);
}

void println(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    printf(fmt, ap);
    va_end(ap);
    printf("\n");
}

void newline()
{
    printf("\n");
}

void fprint(FILE *const stream, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    fprintf(stream, fmt, ap);
    va_end(ap);
}

void fprintln(FILE *const stream, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    fprintf(stream, fmt, ap);
    va_end(ap);
    fprintf(stream, "\n");
}
