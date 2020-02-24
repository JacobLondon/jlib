#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <jlib/io.h>

char *file_read(const char *fname)
{
	char *buf = NULL;
	long length;
	FILE *f = fopen(fname, "rb");

	if (f) {
		fseek(f, 0, SEEK_END);
		length = ftell(f);
		fseek(f, 0, SEEK_SET);
		buf = malloc(length);
		if (buf) {
			(void)fread(buf, 1, length, f);
		}
		fclose(f);
	}
	else {
		return NULL;
	}
	return buf;
}

int file_write(const char *fname, const char *str)
{
	FILE *f = fopen(fname, "wb");
	if (f) {
		fputs(str, f);
		fclose(f);
	}
	else {
		return 0;
	}
	return 1;
}

int file_append(const char *fname, const char *str)
{
	FILE *f = fopen(fname, "ab");
	if (f) {
		fputs(str, f);
		fclose(f);
	}
	else {
		return 0;
	}
	return 1;
}

int file_read_csv(const char *fname, const char *sep, double *mx, size_t y, size_t x)
{
	char *tmp;
	size_t i, j;
	char *raw = file_read(fname);
	
	if (!raw) {
		return 0;
	}

	tmp = strtok(raw, sep);
	if (!tmp) {
		return 0;
	}
	
	for (i = 0; i < y; i++) {
		for (j = 0; j < x; j++) {
			if (sscanf(tmp, "%lf", &mx[i * x + j]) == 0) {
				return 0;
			}
			
			tmp = strtok(NULL, sep);
			if (!tmp) {
				return 0;
			}
		}
	}

	free(raw);
	return 1;
}
