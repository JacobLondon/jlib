#include <string.h>

#include "jlib.h"

static void stringify_test()
{
    char *test = hstring(1000);

    buildstr(test, (char *)"ok");
    buildstr(test, (char *)"ok");
    buildstr(test, (char *)"ok");

    buildcat(test, "wow");
    println("test = %s", test);

    char *lit = hstring_lit("1234567890");
    printf("literal = %s\n", lit);
}

static void array_test()
{
    Array *a = array_new(NULL);
    int b = 10;
    array_push(a, b);
    println("%d", array_read(a, 0, int));
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

static void map_test()
{
    Map *m = map_new(DEF_DEALLOC);
    char a = 'a';
    map_insert(m, "aaaa", a);
    map_insert(m, "aa2a", a);
    map_insert(m, "aa12a", a);
    map_insert(m, "aaaa", a);
    map_insert(m, "a43aaa", a);
    map_insert(m, "aaaa", a);
    map_insert(m, "aafasdaa", a);
    map_insert(m, "aaaa", a);
    //map_insert(m, node_init("aaadsaa", ref(a)));
    HERE(1);
    //map_insert(m, node_init("afaaa", ref(a)));
    //map_insert(m, node_init("adaadaaa", ref(a)));

    println("capacity: %ld\nsize: %ld\n", m->cap, m->size);
    /*
    Node *n;
    map_for_each(m, n) {
        printf("Bucket %ld:", __b);
        printf("%s:\t%c", n->key, val(n->value, char));
    }

    printf("aaaa: ");
    println("%c", val(map_at(m, "aaaa")->value, char));
    */
    HERE(2);
    map_free(m);
    HERE(3);
}

int main()
{
    //array_test();
    //test_match();
    //map_test();
    stringify_test();
}
