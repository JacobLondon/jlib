#include <float.h>
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

	static_assert(10 == 10);
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

static void test_io()
{
	double mx[3][4] = {0};
	int i, j;

	file_read_csv("test/test.csv", ",", (double *)mx, 3, 4);
	/*for (i = 0; i < 4; i++) {
		for (j = 0; j < 3; j++) {
			print("%lf, ", mx[i * 4 + j]);
		}
		newline();
	}*/

	/*printf("%lf %lf %lf %lf\n", mx[0], mx[1], mx[2], mx[3]);
	printf("%lf %lf %lf %lf\n", mx[4], mx[5], mx[6], mx[7]);
	printf("%lf %lf %lf %lf\n", mx[8], mx[9], mx[10], mx[11]);*/
	printf("%lf %lf %lf %lf\n", mx[0][0], mx[0][1], mx[0][2], mx[0][3]);
	printf("%lf %lf %lf %lf\n", mx[1][0], mx[1][1], mx[1][2], mx[1][3]);
	printf("%lf %lf %lf %lf\n", mx[2][0], mx[2][1], mx[2][2], mx[2][3]);
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
	uassert(strfmtlen_d(INT_MAX) == 10);
	uassert(strfmtlen_d(0) == 1);
	uassert(strfmtlen_d(INT_MIN) == 11);
	uassert(strfmtlen_u(0xFFFFFFFFFFFFFFFFULL) == 20);
	uassert(strfmtlen_u(0) == 1);
	uassert(strfmtlen_o(0xFFFFFFFFULL) == 11);
	uassert(strfmtlen_o(0) == 1);
	uassert(strfmtlen_x(0xFFFFFFFFFFFFFFFFULL) == 16);
	uassert(strfmtlen_x(0) == 1);

	uassert(strfmtlen_f(FLT_MAX) == 46);
	uassert(strfmtlen_f(10.0f) == 9);
	uassert(strfmtlen_lf(DBL_MAX) == 316);
	uassert(strfmtlen_lf(1e10) == 18);

	uassert(strfmtlen_e(10.0) == 12);
	uassert(strfmtlen_e(-FLT_MAX) == 13);
	uassert(strfmtlen_e(DBL_MAX) == 13);
	uassert(strfmtlen_e(0.0) == 12);
	uassert(strfmtlen_a(DBL_MAX) == 23);

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
	else if (arg_check(argc, argv, "--io"))
		test_io();
	else {
		puts("Usage:\n--arg\n--debug\n--farray\n--parray\n--py\n--fmap\n--str\n--timer");
	}

	return 0;
}
