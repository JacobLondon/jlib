#ifndef JLIB_PRINT_H
#define JLIB_PRINT_H

#include <stdio.h>

void print(const char *fmt, ...);
void println(const char *fmt, ...);
void newline();

void fprint(FILE *const stream, const char *fmt, ...);
void fprintln(FILE *const stream, const char *fmt, ...);

#endif // JLIB_PRINT_H
