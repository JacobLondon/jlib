#include <string.h>

#include "jlib.h"

static void array_test()
{
    Array a = array_new(NULL);
    int b = 10;
    array_push(a, ref(b));

    printf("%d\n", val(a->buf[0], int));
    array_free(a);
}

int main()
{
    array_test();
    test_match();
}
