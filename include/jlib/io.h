#ifndef JLIB_IO_H
#define JLIB_IO_H

#include <stdio.h>

#define print(Fmt, ...) printf(Fmt, __VA_ARGS__)
#define println(Fmt, ...) printf(Fmt "\n", __VA_ARGS__);
#define fprint(File, Fmt, ...) fprintf(File, Fmt, __VA_ARGS__)
#define fprintln(File, Fmt, ...) fprintf(File, Fmt "\n", __VA_ARGS__);
#define newline() puts("")

char *file_read(const char *fname);
void file_write(const char *fname, const char *cstr);
void file_append(const char *fname, const char *cstr);
void file_read_csv(const char *fname, const char *sep, double *mx, size_t y, size_t x);

#endif /* JLIB_IO_H */