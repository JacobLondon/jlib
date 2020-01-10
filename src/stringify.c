#include <jlib/stringify.h>

char *strnew(const char *lit)
{
	char *build = calloc(strlen(lit) + 1, sizeof(char));
	if (!build) {
		fputs("Error: Could not calloc in strnew\n", stderr);
		exit(-1);
	}
	size_t i;
	for (i = 0; (build[i] = lit[i]); i++)
		;
	
	return build;
}

int streq(const char *cstr0, const char *cstr1)
{
	size_t i;
	for (i = 0; cstr0[i] || cstr1[i]; i++) {
		if (cstr0[i] != cstr1[i])
			return 0;
	}
	return 1;
}

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
			fread(buf, 1, length, f);
		}
		fclose(f);
	}
	else {
		fprintf(stderr, "Error: Could not open file for reading: %s", fname);
		exit(-1);
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
		exit(-1);
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
		exit(-1);
	}
}

CONCAT_LOOKUP(char, c)
CONCAT_LOOKUP(int, d)
CONCAT_LOOKUP(unsigned int, u)
CONCAT_LOOKUP(long int, ld)
CONCAT_LOOKUP(unsigned long int, lu)
CONCAT_LOOKUP(long long int, lld)
CONCAT_LOOKUP(unsigned long long int, llu)
CONCAT_LOOKUP(float, f)
CONCAT_LOOKUP(double, lf)
