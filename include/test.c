#include <string.h>

#include "jlib.h"

static void array_test()
{
    Array *a = array_new(NULL);
    int b = 10;
    array_push(a, ref(b));

    printf("%d\n", val(a->buf[0], int));
    array_free(a);
}


/**
 * test
 */

static char *rand_string(char *str, size_t size)
{
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJK";
    if (size) {
        --size;
        for (size_t n = 0; n < size; n++) {
            int key = rand() % (int) (sizeof charset - 1);
            str[n] = charset[key];
        }
        str[size] = '\0';
    }
    return str;
}

static map_test()
{
    Map *m = map_new(NULL);
    char a = 'a';

    map_insert(m, node_init("aaaa", ref(a)));
    map_insert(m, node_init("aa2a", ref(a)));
    map_insert(m, node_init("aa12a", ref(a)));
    map_insert(m, node_init("aaaa", ref(a)));
    map_insert(m, node_init("a43aaa", ref(a)));
    map_insert(m, node_init("aaaa", ref(a)));
    map_insert(m, node_init("aafasdaa", ref(a)));
    map_insert(m, node_init("aaaa", ref(a)));
    map_insert(m, node_init("aaadsaa", ref(a)));
    map_insert(m, node_init("afaaa", ref(a)));
    map_insert(m, node_init("adaadaaa", ref(a)));

    printf("capacity: %d\nsize: %d\n", m->cap, m->size);

    Node *n;
    map_for_each(m, n) {
        printf("Bucket %d:", __b);
        print("%s:\t%c", n->key, val(n->value, char));
    }

    printf("aaaa: ");
    printf("%d\n", val(map_at(m, "aaaa")->value, char));

    map_free(m);
}

int main()
{
    //array_test();
    //test_match();
    map_test();
}
