#include <stdlib.h>
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
		fprintf(stderr, "Error: Could not open file for reading: %s", fname);
		exit(1);
	}
	return buf;
}

void file_write(const char *fname, const char *cstr)
{
	FILE *f = fopen(fname, "wb");
	if (f) {
		fputs(cstr, f);
		fclose(f);
	}
	else {
		fprintf(stderr, "Error: Could not open file for writing: %s", fname);
		exit(1);
	}
}

void file_append(const char *fname, const char *cstr)
{
	FILE *f = fopen(fname, "ab");
	if (f) {
		fputs(cstr, f);
		fclose(f);
	}
	else {
		fprintf(stderr, "Error: Could not open file for appending: %s", fname);
		exit(1);
	}
}

void file_read_csv(const char *fname, const char *sep, double *mx, size_t y, size_t x)
{
	char *raw = file_read(fname);
	char *tmp;
	size_t i, j;

	tmp = strtok(raw, sep);
	if (!tmp) {
		goto Error;
	}
	
	for (i = 0; i < y; i++) {
		for (j = 0; j < x; j++) {
			if (sscanf(tmp, "%lf", &mx[i * x + j]) == 0) {
				goto Error;
			}
			
			tmp = strtok(NULL, sep);
			if (!tmp) {
				goto Error;
			}
		}
	}

	free(raw);
	return;
Error:
	fprintf(stderr, "Error: Unable to read csv.\n");
	exit(1);
}
