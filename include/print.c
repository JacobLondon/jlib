#include <stdarg.h>
#include <string.h>

#include "print.h"

void print(const char *__restrict__ fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    printf(fmt, ap);
    va_end(ap);
}

void println(const char *__restrict__ fmt, ...)
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

void fprint(FILE *__restrict__ stream, const char *__restrict__ fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    fprintf(stream, fmt, ap);
    va_end(ap);
}

void fprintln(FILE *__restrict__ stream, const char *__restrict__ fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    fprintf(stream, fmt, ap);
    va_end(ap);
    fprintf(stream, "\n");
}
