#include <string.h>
#include <jlib/jlib.h>

static void array_test()
{
    Array *a = jlib_array_new(NULL);
    int b = 10;
    jlib_array_push(a, b);
    println("%d", jlib_array_read(a, 0, int));
    jlib_array_free(a);
}

static void arg_test()
{
    int argc = 4;
    char *argv[4] = {
        "--count",
        "10",
        "-c",
        "-v",
    };

    struct def_s {
        int count_num;
        bool c;
        bool v;
    } defs;

    // get args
    defs.c = jlib_arg_check(argc, argv, "-c");
    defs.v = jlib_arg_check(argc, argv, "-v");

    char *tmp;
    if ((tmp = jlib_arg_get(argc, argv, "--count")))
        jlib_strtot(defs.count_num, tmp);
    else
        defs.count_num = 0;
    
    // print found args
    println("%d\t%d\t%d", defs.count_num, defs.c, defs.v);
}


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

#if 0
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
#endif

static void stringify_test()
{
    char *test = jlib_string_new(1000);

    jlib_strcat(test, (char *)"ok");
    jlib_strcat(test, (char *)"ok");
    jlib_strcat(test, (char *)"ok");

    jlib_strcat(test, "wow");
    println("test = %s", test);

    char *lit = jlib_string_from("1234567890");
    printf("literal = %s\n", lit);

    int a;
    jlib_strtot(a, "10");
    println("a = %d", a);
}

int main(int argc, char **argv)
{
    if (jlib_arg_check(argc, argv, "--array"))
        array_test();
    else if (jlib_arg_check(argc, argv, "--arg"))
        arg_test();
    /*else if (arg_check(argc, argv, "--map"))
        map_test();*/
    else if (jlib_arg_check(argc, argv, "--string"))
        stringify_test();
    else {
        puts("Usage:\n--array\n--arg\n--map\n--string");
    }

    return 0;
}
