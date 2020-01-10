#include <string.h>
#include <jlib/jlib.h>

static void array_test()
{
	struct array_s *a = array_new(NULL);
	int b = 10;
	array_push(a, b);
	println("%d", array_read(a, 0, int));
	array_free(a);
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

static void stringify_test()
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
	if (arg_check(argc, argv, "--array"))
		array_test();
	else if (arg_check(argc, argv, "--arg"))
		arg_test();
	else if (arg_check(argc, argv, "--string"))
		stringify_test();
	else {
		puts("Usage:\n--array\n--arg\n--string");
	}

	return 0;
}
