#ifndef JLIB_IO_H
#define JLIB_IO_H

/**
 * Read a file into an allocated string
 * Size out variable from read file length
 */
char *file_read(const char *fname, size_t *size);

/**
 * Read a file into a buffer of given bytes
 */
int file_read_into(const char *fname, size_t bytes, void *buf);

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

/**
 * Perform a hexdump of a buffer
 */
void memdump(void *buf, size_t size);

/**
 * Perform a malloc with overrun check. Used with memdump when
 * an overrun occurs.
 */
void *check_malloc(size_t bytes);

void check_free(void *buf);

#endif /* JLIB_IO_H */
