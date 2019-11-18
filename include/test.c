#include <string.h>

#include "jlib.h"

#define match1(fn, search, value, enclosed) do { \
    if (fn(search, value)) \
        enclosed \
    else {} \
} while (0)

#define match2(fn, search, value, enclosed, ...) do { \
    if (fn(search, value)) \
        enclosed \
    else \
        match1(fn, search, ...); \
} while (0)

#define match(fn, search, value1, enclosed1, value2, enclosed2) do { \
    if (fn(search, value1)) \
        enclosed1 \
    else if (fn(search, value2)) \
        enclosed2 \
} while (0)

static int cmpnum(int a, int b) { return a == b; }
static int cmpstr(char *a, char *b) { return strcmp(a, b) == 0; }

int main()
{
    
    match(cmpnum, 10,
    5, {
        printf("10 == 5!\n");
    },
    10, {
        printf("10 == 19\n");
    });

    match(cmpstr, "wwwwwwooooww",
    "nope", {
        printf("wwwwwwooooww == nope\n");
    },
    "wwwwwwooooww", {
        printf("wwwwwwooooww == wwwwwwooooww\n");
    });
    
    /*
    match2(cmpnum, 10,
    5, {
        printf("2\n");
    },
    10, {
        printf("1\n");
    });

    match1(cmpnum, 10,
    10, {
        printf("3\n");
    });
    */

    //print_test();
    allocator_test();
    //vector_test();
}
