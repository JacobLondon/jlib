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

static void test_farray()
{
	struct farray *a = farray_new(sizeof(int));
	farray_push(a, 10, int);
	farray_push(a, 11, int);
	farray_push(a, 12, int);

	println("%d", farray_read(a, 0, int));
	println("%d", farray_read(a, 1, int));
	println("%d", farray_read(a, 2, int));

	farray_write(a, 2, 10, int);
	println("%d", farray_read(a, 2, int));
	farray_pop(a);
	/* will clear the last item with zeros */
	println("%d", farray_read(a, 2, int));
	farray_resize(a, 50);

	farray_free(a);
}

static void test_parray()
{
	struct parray *a = parray_new(NULL);
	int b = 10;
	int c = 11;
	int d = 12;
	parray_push(a, b);
	parray_push(a, c);
	parray_push(a, d);
	println("%d", parray_read(a, 0, int));
	println("%d", parray_read(a, 1, int));
	println("%d", parray_read(a, 2, int));
	parray_write(a, 2, b);
	println("%d", parray_read(a, 2, int));
	parray_pop(a);
	/* will segfault, index 2 was just NULL'ed */
	/* println("%d", parray_read(a, 2, int)); */
	println("%d", parray_read(a, 1, int));
	parray_resize(a, 50);
	parray_free(a);
}

static void test_fmap()
{
	struct fmap *m = fmap_new(sizeof(int));

	fmap_write(m, "a", 10, int);
	fmap_write(m, "b", 11, int);
	fmap_write(m, "c", 12, int);
	println("%d", fmap_read(m, "a", int));
	println("%d", fmap_read(m, "b", int));
	println("%d", fmap_read(m, "c", int));
	/* will print 0, as it's empty */
	println("%d", fmap_read(m, "d", int));

	println("'a' in map? %d", fmap_check(m, "a"));
	println("'d' in map? %d", fmap_check(m, "d"));

	fmap_write(m, "c", 12, int);
	fmap_write(m, "d", 12, int);
	fmap_write(m, "e", 12, int);
	fmap_write(m, "f", 12, int);
	fmap_write(m, "g", 12, int);
	fmap_write(m, "h", 12, int);
	fmap_write(m, "i", 12, int);
	fmap_write(m, "j", 12, int);
	
	newline();
	println("Resizing...\nold cap: %ld", m->cap);
	fmap_grow_to(m, 100);
	println("new cap: %ld", m->cap);
	println("'a' still in map? %d", fmap_check(m, "a"));
	puts("Removing A...");
	fmap_remove(m, "a");
	println("'a' still in map? %d", fmap_check(m, "a"));

	puts("Removing 'z' (not in)");
	fmap_remove(m, "z");

	puts("Freeing...");
	fmap_free(m);
	puts("Success!");
}

static void test_py()
{
	int x[5] = {0, 1, 2, 3, 4};
	char *py_x = python_iatoa("x", x, 5);
	println("%s", py_x);

	python_init();
	/*python_run("from time import time,ctime\n"
						"print('Today is',ctime(time()))\n");*/
	python_run(
		"import numpy as np\n"
	);
	python_run(py_x);
	python_run(
		"x = np.array(x)\n"
		"print(x)\n"
	);
	python_exit();
}

static void test_str()
{
	char *test = calloc(1000, 1);

	strcat(test, "ok");
	strcat(test, "ok");
	strcat(test, "ok");
	strcat(test, "wow");
	println("test = %s", test);

	char *lit = strdup("1234567890");
	print("literal = %s\n", lit);

	char *build = strcatf(NULL, "%d %lf %s", 1, 177.2, "tester");
	build = strcatf(build, " %llu", 10000ULL);
	println("%s", build);
}

static void test_timer()
{
	unsigned long i;
	double dur;

	timer_rst();
	for (i = 0; i < 50000UL; i++)
		print("%lu\r", i);
	dur = timer_lap();
	
	newline();
	println("Duration: %lf", dur);
}

int main(int argc, char **argv)
{
	if (arg_check(argc, argv, "--arg"))
		test_arg();
	else if (arg_check(argc, argv, "--debug"))
		test_debug();
	else if (arg_check(argc, argv, "--farray"))
		test_farray();
	else if (arg_check(argc, argv, "--parray"))
		test_parray();
	else if (arg_check(argc, argv, "--fmap"))
		test_fmap();
	else if (arg_check(argc, argv, "--py"))
		test_py();
	else if (arg_check(argc, argv, "--str"))
		test_str();
	else if (arg_check(argc, argv, "--timer"))
		test_timer();
	else {
		puts("Usage:\n--arg\n--debug\n--farray\n--parray\n--py\n--fmap\n--str\n--timer");
	}

	return 0;
}
