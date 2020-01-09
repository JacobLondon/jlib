#include <jlib/stringify.h>
#include <jlib/alloc.h>

char *jlib_string_from(const char *literal)
{
    size_t size = strlen(literal);
    char *build = jlib_string_new(size + 1);
    size_t i = 0;
    while ((build[i] = literal[i]))
        i++;
    
    return build;
}

void jlib_string_concat(char *target, void *src, size_t size)
{
    size_t start = strlen(target);
    size_t length = start + size + 1;
    
    // realloc characters
    char *tmp;
    jlib_try_realloc(target, tmp, length, "Coult not realloc in build_concat");

    for (size_t i = 0; i < size; i++) {
        target[start + i] = ((char *)src)[i];
    }
}

int jlib_string_fast_eq(const char *str0, const char *str1)
{
    for (size_t i = 0; str0[i] != '\0' || str1[i] != '\0'; i++) {
        if (str0[i] != str1[i])
            return 0;
    }
    return 1;
}

char *jlib_string_fread(const char *fname)
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
    return buf;
}

void jlib_string_fwrite(const char *fname, const char *cstr)
{
    FILE *f = fopen(fname, "wb");
    if (f) {
        fputs(cstr, f);
        fclose(f);
    }
}

void jlib_string_fappend(const char *fname, const char *cstr)
{
    FILE *f = fopen(fname, "ab");
    if (f) {
        fputs(cstr, f);
        fclose(f);
    }
}

JLIB_STRING_LOOKUP(char, c)
JLIB_STRING_LOOKUP(int, d)
JLIB_STRING_LOOKUP(unsigned int, u)
JLIB_STRING_LOOKUP(long int, ld)
JLIB_STRING_LOOKUP(unsigned long int, lu)
JLIB_STRING_LOOKUP(long long int, lld)
JLIB_STRING_LOOKUP(unsigned long long int, llu)
JLIB_STRING_LOOKUP(float, f)
JLIB_STRING_LOOKUP(double, lf)
JLIB_STRING_LOOKUP(char *, s)
