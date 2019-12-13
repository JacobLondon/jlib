#ifndef JLIB_PRINT_H
#define JLIB_PRINT_H

#include <stdio.h>

void print(const char *__restrict__ fmt, ...);
void println(const char *__restrict__ fmt, ...);
void newline();

void fprint(FILE *__restrict__ const stream, const char *__restrict__ fmt, ...);
void fprintln(FILE *__restrict__ const stream, const char *__restrict__ fmt, ...);

#endif // JLIB_PRINT_H
