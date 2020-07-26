#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <jlib/io.h>

char *file_read(const char *fname, size_t *size)
{
	if (!fname) {
		return NULL;
	}

	char *buf;
	size_t bytes;
	long length;

	FILE *f = fopen(fname, "rb");
	assert(f);

	fseek(f, 0, SEEK_END);
	length = ftell(f);
	fseek(f, 0, SEEK_SET);

	buf = malloc(length + 1);
	assert(buf);
	
	bytes = fread(buf, 1, length, f);
	buf[bytes] = '\0';
	if (size) {
		*size = bytes;
	}

	fclose(f);
	return buf;
}

int file_write(const char *fname, const char *str)
{
	FILE *f = fopen(fname, "wb");
	if (!f) {
		return 0;
	}

	fputs(str, f);
	fclose(f);
	return 1;
}

int file_append(const char *fname, const char *str)
{
	FILE *f = fopen(fname, "ab");
	if (!f) {
		return 0;
	}

	fputs(str, f);
	fclose(f);
	return 1;
}

int file_read_csv(const char *fname, const char *sep, double *mx, size_t y, size_t x)
{
	assert(fname);
	assert(sep);
	assert(mx);

	char *tmp;
	size_t i, j;
	char *raw = file_read(fname, NULL);
	
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

#include <ctype.h>

void memdump(void *buf, size_t size)
{
	#define X_WIDTH 10

	size_t i, j;
	unsigned char tmp;
	unsigned char data[X_WIDTH];
	memset(data, 0, sizeof(data));

	for (i = 0; i < size; i++) {
		tmp = ((unsigned char *)buf)[i];
		printf("%02X ", tmp);

		if (isprint(tmp)) {
			data[i % X_WIDTH] = tmp;
		}
		else {
			data[i % X_WIDTH] = '.';
		}

		if ((i + 1) % X_WIDTH == 0) {
			printf("\t");
			for (j = 0; j < X_WIDTH; j++) {
				printf("%c", data[j]);
			}
			memset(data, 0, sizeof(data));
			printf("\n");
		}
	}

	for (; i % X_WIDTH != 0; i++) {
		printf("00 ");
		data[i % X_WIDTH] = '.';
	}

	if (i % X_WIDTH == 0) {
		printf("\t");
		for (j = 0; j < X_WIDTH; j++) {
			printf("%c", data[j]);
		}
		memset(data, 0, sizeof(data));
	}
	printf("\n");

	#undef X_WIDTH
}

#define CHECK_MALLOC_EXTRA 32

void *check_malloc(size_t bytes)
{
	void *buf = malloc(CHECK_MALLOC_EXTRA + bytes + CHECK_MALLOC_EXTRA);
	if (!buf) {
		return NULL;
	}
	memset(buf, 0, CHECK_MALLOC_EXTRA + bytes + CHECK_MALLOC_EXTRA);
	*((size_t *)buf) = bytes;
	return ((char *)buf + CHECK_MALLOC_EXTRA);
}

void check_free(void *buf)
{
	char clear[CHECK_MALLOC_EXTRA] = { 0 };
	size_t size = *(size_t *)((char *)buf - CHECK_MALLOC_EXTRA);

	/* check before */
	if (memcmp(buf - CHECK_MALLOC_EXTRA + sizeof(size_t), &clear[sizeof(size_t)], CHECK_MALLOC_EXTRA - sizeof(size_t)) != 0) {
		memdump(buf - CHECK_MALLOC_EXTRA, CHECK_MALLOC_EXTRA + size + CHECK_MALLOC_EXTRA);
	}
	/* check after */
	else if (memcmp(buf + size, clear, CHECK_MALLOC_EXTRA)) {
		memdump((char *)buf - CHECK_MALLOC_EXTRA, CHECK_MALLOC_EXTRA + size + CHECK_MALLOC_EXTRA);
	}
	free(buf - CHECK_MALLOC_EXTRA);
}

#undef CHECK_MALLOC_EXTRA
