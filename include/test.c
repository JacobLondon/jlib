#include <string.h>

#include "jlib.h"

static int cmpnum(int a, int b) { return a == b; }
static int cmpstr(char *a, char *b) { return strcmp(a, b) == 0; }

static void test_match()
{
   match(3, cmpstr, "match!",
   "skip", {
       printf("1\n");
   },
   "match!", {
       printf("2\n");
   },
   "ignored", {
       printf("3\n");
   });
}

static void array_test()
{
    jlib_init();

    #define array jlib.array

    Array *a = array.new(NULL);
    int b = 10;
    array.push(a, ref(b));

    printf("%d\n", val(a->buf[0], int));
    array.free(a);
}

int main()
{
    array_test();
    test_match();
}
