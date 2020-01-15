#include <string.h>
#include <jlib/jlib.h>

static void test_arg()
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
		int c;
		int v;
	} defs;

	/* get args */
	defs.c = arg_check(argc, argv, "-c");
	defs.v = arg_check(argc, argv, "-v");

	char *tmp;
	if ((tmp = arg_get(argc, argv, "--count")))
		defs.count_num = strtol(tmp, NULL, 10);
	else
		defs.count_num = 0;
	
	/* print found args */
	println("%d\t%d\t%d", defs.count_num, defs.c, defs.v);
}

static void test_array()
{
	struct array_s *a = array_new(NULL);
	int b = 10;
	int c = 11;
	int d = 12;
	array_push(a, b);
	array_push(a, c);
	array_push(a, d);
	println("%d", array_read(a, 0, int));
	println("%d", array_read(a, 1, int));
	println("%d", array_read(a, 2, int));
	array_write(a, 2, b);
	println("%d", array_read(a, 2, int));
	array_pop(a);
	println("%d", array_read(a, 2, int));
	array_free(a);
}

static void test_debug()
{
	HERE(1);
	HERE_OFF;
	HERE(2);
	HERE_ON;
	HERE(3);

	assert_static(10 == 10);
	halt("Static assert successful");
}

static void test_stringify()
{
	char *test = calloc(1000, 1);

	strcat(test, "ok");
	strcat(test, "ok");
	strcat(test, "ok");
	strcat(test, "wow");
	concat_lld(test, 100L);
	println("test = %s", test);

	char *lit = strnew("1234567890");
	print("literal = %s\n", lit);
}

int main(int argc, char **argv)
{
	if (arg_check(argc, argv, "--arg"))
		test_arg();
	else if (arg_check(argc, argv, "--array"))
		test_array();
	else if (arg_check(argc, argv, "--debug"))
		test_debug();
	else if (arg_check(argc, argv, "--string"))
		test_stringify();
	else {
		puts("Usage:\n--array\n--arg\n--string");
	}

	return 0;
}
