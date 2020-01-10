#ifndef JLIB_PRINT_H
#define JLIB_PRINT_H

#include <stdio.h>

#define print(Fmt, ...) printf(Fmt, __VA_ARGS__)

#define println(Fmt, ...) do { \
	printf(Fmt, __VA_ARGS__); \
	printf("\n"); \
} while (0)

#define newline() printf("\n")

#define fprint(File, Fmt, ...) fprintf(File, Fmt, __VA_ARGS__)
#define fprintln(File, Fmt, ...) do { \
	printf(File, Fmt, __VA_ARGS__); \
	printf("\n"); \
} while (0)

#endif /* JLIB_PRINT_H */