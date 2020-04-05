#ifndef JLIB_IO_H
#define JLIB_IO_H

/**
 * Read a file into an allocated string
 * Size out variable from read file length
 */
char *file_read(const char *fname, size_t *size);

/**
 * Overwrite all data in a file with a string
 */
int file_write(const char *fname, const char *str);

/**
 * Append a string to a file
 */
int file_append(const char *fname, const char *str);

/**
 * Read a file as a 'sep' seperated file into a matrix of doubles
 */
int file_read_csv(const char *fname, const char *sep, double *mx, size_t y, size_t x);

#endif /* JLIB_IO_H */