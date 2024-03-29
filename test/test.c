#include <float.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <jlib/jlib.h>

static void test_arg(void)
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
	defs.c = arg_check(argc, argv, "-c", NULL);
	defs.v = arg_check(argc, argv, "-v", NULL);

	char *tmp;
	if ((tmp = arg_get(argc, argv, NULL, "--count"))) {
		(void)sscanf(tmp, "%d", &defs.count_num);
	}
	else
		defs.count_num = 0;
	
	/* print found args */
	printf("%d\t%d\t%d\n", defs.count_num, defs.c, defs.v);
}

char myarray[10][10] = {
	"          ",
	"######### ",
	"          ",
	" #########",
	"          ",
	"######### ",
	"          ",
	" #########",
	"          ",
	"######### ",
};

static bool my_obstacle_cb(int i, int j)
{
	return myarray[i][j] == '#';
}

static void test_astar(void)
{
	unsigned xs[100] = { 0 };
	unsigned ys[100] = { 0 };
	long size;
	int i, j;

	astar_init(10, 10, my_obstacle_cb);
	size = astar_path(xs, ys, 100, 0, 0, 9, 9);
	printf("Path is %zu units\n", size);

	for (i = 0; i < size; i++) {
		myarray[ys[i]][xs[i]] = '.';
	}

	for (i = 0; i < 10; i++) {
		for (j = 0; j < 10; j++) {
			printf("%c ", myarray[i][j]);
		}
		printf("\n");
	}

	astar_cleanup();
}

static void test_debug(void)
{
	HERE(1);
	HERE(2);
	HERE(3);

	static_assert(10 == 10, "Must be valid");
	halt("Static assert successful");
}

static void test_gc(void)
{
	size_t i;
	struct gc *gc = gc_new();

	for (i = 0; i < 500; i++) {
		gc_alloc(gc, sizeof(int));
	}
	printf("Size 1: %zu\n", gc->size);
	gc_mark(gc);

	for (i = 0; i < 500; i++) {
		gc_alloc(gc, sizeof(int));
	}
	printf("Size 2: %zu\n", gc->size);

	gc_collect(gc);
	printf("Size 3: %zu\n", gc->size);
	gc_collect(gc);
	printf("Size 4: %zu\n", gc->size);
	gc_free(gc);
}

static void test_farray(void)
{
	struct farray *a = farray_new(sizeof(int));
	size_t i;
	for (i = 0; i < 100000; i++) {
		farray_push(a, 10, int);
	}

	farray_free(a);
}

static void test_parray(void)
{
	struct parray *a = parray_new(NULL);
#if 0
	int b = 10;
	int c = 11;
	int d = 12;
	parray_push(a, b);
	parray_push(a, c);
	parray_push(a, d);
	printf("%d\n", parray_read(a, 0, int));
	printf("%d\n", parray_read(a, 1, int));
	printf("%d\n", parray_read(a, 2, int));
	parray_write(a, 2, b);
	printf("%d\n", parray_read(a, 2, int));
	parray_pop(a);
	/* will segfault, index 2 was just NULL'ed */
	/* printf("%d\n", parray_read(a, 2, int)); */
	printf("%d\n", parray_read(a, 1, int));
#endif
	parray_resize(a, 50);
	parray_free(a);
}

static void test_fmap(void)
{
	struct fmap *m = fmap_new(sizeof(int));
	char buf[32] = {0};

	size_t i;
	for (i = 0; i < 100000; i++) {
		buf[i % 31]++;
		fmap_write(m, buf, 10, int);
	}
	
	volatile char *key;
	volatile int value;
	fmap_for_each(m, key, value, int, {
		(void)key;
		(void)value;
	});

	fmap_write(m, "a", 10, int);
	fmap_write(m, "b", 11, int);
	fmap_write(m, "c", 12, int);
	printf("%d\n", fmap_read(m, "a", int));
	printf("%d\n", fmap_read(m, "b", int));
	printf("%d\n", fmap_read(m, "c", int));
	/* will print 0, as it's empty */
	printf("%d\n", fmap_read(m, "d", int));

	printf("'a' in map? %d\n", fmap_check(m, "a"));
	printf("'d' in map? %d\n", fmap_check(m, "d"));

	fmap_write(m, "c", 12, int);
	fmap_write(m, "d", 12, int);
	fmap_write(m, "e", 12, int);
	fmap_write(m, "f", 12, int);
	fmap_write(m, "g", 12, int);
	fmap_write(m, "h", 12, int);
	fmap_write(m, "i", 12, int);
	fmap_write(m, "j", 12, int);
	
	printf("\n");
	printf("'a' still in map? %d\n", fmap_check(m, "a"));
	printf("Removing A...\n");
	fmap_remove(m, "a");
	printf("'a' still in map? %d\n", fmap_check(m, "a"));

	printf("Clearing 'z' (not in)\n");
	fmap_remove(m, "z");

	printf("Freeing...\n");
	fmap_free(m);
	printf("Success!\n");
}

static void test_io(void)
{
	double mx[3][4] = {0};

	file_read_csv("test/test.csv", ",", (double *)mx, 3, 4);
	printf("%lf %lf %lf %lf\n", mx[0][0], mx[0][1], mx[0][2], mx[0][3]);
	printf("%lf %lf %lf %lf\n", mx[1][0], mx[1][1], mx[1][2], mx[1][3]);
	printf("%lf %lf %lf %lf\n", mx[2][0], mx[2][1], mx[2][2], mx[2][3]);
}

static void test_check(void)
{
	int *buf = check_malloc(sizeof(int) * 10);
	buf[10] = 'F';
	check_free(buf);
}

static void test_list(void)
{
	struct list_node *tmp;
	struct list_node **cursor;
	struct list *mylist = list_new(free);

	list_push_front(mylist, strdup("1"));
	printf("Second element: %p\n", list_tail(mylist));
	tmp = list_push_back(mylist, strdup("3"));
	printf("Second element: %p\n", list_tail(mylist));
	tmp = list_insert_next(mylist, tmp, strdup("4"));
	list_insert_next(mylist, mylist->head, strdup("2"));
	list_push_back(mylist, strdup("4"));
	list_push_front(mylist, strdup("0"));
	list_pop_front(mylist);
	list_pop_back(mylist);
	list_remove(mylist, tmp);

	tmp = list_find(mylist, "1", (int (*)(void *, void *))strcmp);
	printf("Found '1'?: %p\n", tmp);
	list_remove(mylist, tmp);
	tmp = list_find(mylist, "1", (int (*)(void *, void *))strcmp);
	printf("Found '1' after remove?: %p\n", tmp);

	for (cursor = list_iter_begin(mylist); !list_iter_done(cursor); list_iter_continue(&cursor)) {
		printf("%s\n", (char *)list_iter_value(cursor));
	}

	while (mylist->head != NULL) {
		list_remove(mylist, mylist->head);
	}

	list_push_front(mylist, strdup("1"));
	printf("Second element: %p\n", list_tail(mylist));
	tmp = list_push_back(mylist, strdup("3"));
	printf("Second element: %p\n", list_tail(mylist));
	tmp = list_insert_next(mylist, tmp, strdup("4"));
	list_insert_next(mylist, mylist->head, strdup("2"));
	list_push_back(mylist, strdup("4"));
	list_push_front(mylist, strdup("0"));
	list_pop_front(mylist);
	list_pop_back(mylist);
	list_remove(mylist, tmp);

	tmp = list_find(mylist, "1", (int (*)(void *, void *))strcmp);
	printf("Found '1'?: %p\n", tmp);
	list_remove(mylist, tmp);
	tmp = list_find(mylist, "1", (int (*)(void *, void *))strcmp);
	printf("Found '1' after remove?: %p\n", tmp);

	for (cursor = list_iter_begin(mylist); !list_iter_done(cursor); list_iter_continue(&cursor)) {
		printf("%s\n", (char *)list_iter_value(cursor));
	}

	printf("Done\n");
	list_free(mylist);
}

static void test_py(void)
{
#if 0
	int x[5] = {0, 1, 2, 3, 4};
	char *py_x = python_iatoa("x", x, 5);
	printf("%s\n", py_x);

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
#endif
}

static void test_str(void)
{
	#if 0
	assert(strfmtlen_d(INT_MAX) == 10);
	assert(strfmtlen_d(0) == 1);
	assert(strfmtlen_d(INT_MIN) == 11);
	assert(strfmtlen_u(0xFFFFFFFFFFFFFFFFULL) == 20);
	assert(strfmtlen_u(0) == 1);
	assert(strfmtlen_o(0xFFFFFFFFULL) == 11);
	assert(strfmtlen_o(0) == 1);
	assert(strfmtlen_x(0xFFFFFFFFFFFFFFFFULL) == 16);
	assert(strfmtlen_x(0) == 1);

	assert(strfmtlen_f(FLT_MAX) == 46);
	assert(strfmtlen_f(10.0f) == 9);
	assert(strfmtlen_lf(DBL_MAX) == 316);
	assert(strfmtlen_lf(1e10) == 18);

	assert(strfmtlen_e(10.0) == 12);
	assert(strfmtlen_e(-FLT_MAX) == 13);
	assert(strfmtlen_e(DBL_MAX) == 13);
	assert(strfmtlen_e(0.0) == 12);
	assert(strfmtlen_a(DBL_MAX) == 23);
	#endif
	//char *lit = strdup("1234567890");
	//printf("literal = %s\n", lit);

	/*char *build = NULL;
	strcatf(&build, "%d %lf %s", 1, 177.2, "tester");
	strcatf(&build, " %llu", 10000ULL);
	strcatf(&build, " %lf", 2e123);
	printf("%s\n", build);
	free(build);*/

	/*char *build = strdup("1234abc1234abc1234abc");
	if (streplace(&build, "abc", "yesno")) {
		printf("%s\n", build);
	}
	else {
		printf("Failure\n");
	}*/

	//char *s = strndup("0123456789", 3);
	//printf("%s\n", s);
	//free(s);

	char *words = "Hello, how is your day today? I'm hoping you are doing well!";
	char **split = strsplit(words, " ,?!");
	size_t i;

	printf("Words: %s\n", words);
	printf("Split: ");
	
	for (i = 0; split[i]; i++) {
		printf("%s\n", split[i]);
	}
	printf("\n");
	strsplit_free(split);

}

static void test_token(void)
{
#if 0
	FILE *fp = fopen("test/tokenize.txt", "r");
	assert(fp);

	int ch;
	int idx = 0;
	char buf[256] = { 0 };
	char *bufp = buf;

	size_t lineno = 1;
	size_t colno = 1;
	struct parray *tokens = parray_new((void (*)(void *))token_free);

	enum tokid {
		WHITESPACE,
		WORD,
		NUM,
		PERIOD,
		UNKNOWN,
	};

	while (!feof(fp)) {
		ch = getc(fp);

		if (bufp >= buf + sizeof(buf) - 1) {
			fprintf(stderr, "Error: Token too large %s\n", buf);
			break;
		}

		if (isspace(ch)) {
			if (ch == '\n') {
				lineno++;
				colno = 1;
			}
			else if (ch == '\r') {
				colno = 1;
			}
			else {
				colno++;
			}
		}
		else if (isalpha(ch)) {
			*bufp++ = ch;
			colno++;
			ch = getc(fp);
			if (!isalpha(ch)) {
				parray_push(tokens, token_new(WORD, buf, idx, lineno, colno - strlen(buf)));
				memset(buf, 0, sizeof(buf));
				bufp = buf;
			}
			ungetc(ch, fp);
		}
		else if (isdigit(ch)) {
			*bufp++ = ch;
			colno++;
			ch = getc(fp);
			if (!isdigit(ch)) {
				parray_push(tokens, token_new(NUM, buf, idx, lineno, colno - strlen(buf)));
				memset(buf, 0, sizeof(buf));
				bufp = buf;
			}
			ungetc(ch, fp);
		}
		else if (ch == '.') {
			parray_push(tokens, token_new(PERIOD, ".", idx, lineno, colno));
			colno++;
		}
		else {
			buf[0] = ch;
			parray_push(tokens, token_new(UNKNOWN, buf, idx, lineno, colno));
			buf[0] = 0;
			colno++;
		}
		idx++;
	}

	size_t i;
	for (i = 0; i < tokens->size; i++) {
		token_put(tokens->buf[i]);
	}

	fclose(fp);
	parray_free(tokens);
#endif
}

enum MyTokenIds {
	MY_WORD,
	MY_PERIOD,
	MY_NUMBER,
	MY_WHITESPACE,
	MY_UNKNOWN,
	MY_END,
};

static struct token test_tok_gettok(struct tokenizer *ctx)
{
	struct token rv;
	char ch;

	if (isalpha(ctx->cursor[0])) {
		rv.symbol = ctx->cursor;
		rv.id = MY_WORD;
		for (rv.len = 1; (ch = tokenizer_next(ctx)) && isalpha(ch); rv.len++);
	}
	else if (isdigit(ctx->cursor[0])) {
		rv.symbol = ctx->cursor;
		rv.id = MY_NUMBER;
		for (rv.len = 1; (ch = tokenizer_next(ctx)) && isdigit(ch); rv.len++);
	}
	else if (isspace(ctx->cursor[0])) {
		rv.symbol = ctx->cursor;
		rv.id = MY_WHITESPACE;
		for (rv.len = 1; (ch = tokenizer_next(ctx)) && isspace(ch); rv.len++);
	}
	else if (ctx->cursor[0] == '.') {
		rv.symbol = ctx->cursor;
		rv.id = MY_PERIOD;
		rv.len = 1;
		(void)tokenizer_next(ctx);
	}
	else if (ctx->cursor[0] == 0) {
		rv.symbol = NULL;
		rv.id = MY_END;
		rv.len = 1;
	}
	else {
		rv.symbol = ctx->cursor;
		rv.id = MY_UNKNOWN;
		rv.len = 1;
		(void)tokenizer_next(ctx);
	}

	return rv;
}

static void test_tok(void)
{
	struct tokenizer context;
	struct token tok;
	size_t size;
	char *text = file_read("test/tokenize.txt", &size);
	tokenizer_new(text, size, test_tok_gettok, &context);

	for (tok = tokenizer_gettok(&context);
	     tok.id != MY_END;
	     tok = tokenizer_gettok(&context))
	{
		token_puts(&tok);
	}
	printf("%zu tokens found\n", tokenizer_count(&context));
	tokenizer_del(&context);
	free(text);
}

static void test_tree_puts(void *value, size_t depth)
{
	char *str = (char *)value;
	assert(value);

	for ( ; depth > 0; depth--) {
		printf("    ");
	}
	printf("%s\n", str);
	fflush(stdout);
}

static void test_tree(void)
{
	struct tree_node *root = tree_node_new(strdup("Hello"));
	struct tree_node *tmp;
	struct tree_node *other;
	tmp = tree_node_append(root, strdup("World"));
	tree_node_append(tmp, strdup("Test"));
	tree_node_append(tmp, strdup("1234"));
	tmp = tree_node_prepend(root, strdup("Other"));
	tree_node_append(tmp, strdup("Last"));

	tmp = tree_node_find(root, "Other", (int (*)(void *, void *))strcmp);
	if (tmp) {
		printf("Found!\n");
	}
	else {
		printf("Not found!\n");
	}

	other = tree_node_new(strdup("NEW BRANCH"));
	tree_node_insert(tmp, other);

	tree_node_map(root, test_tree_puts);

	tmp = tree_node_find(root, "not here mate", (int (*)(void *, void *))strcmp);
	if (tmp) {
		printf("Found!\n");
	}
	else {
		printf("Not found!\n");
	}

	tree_node_del(root, free);
}

static void test_timer(void)
{
	unsigned long i;
	double dur;

	timer_rst();
	for (i = 0; i < 50000UL; i++)
		printf("%lu\r", i);
	dur = timer_lap();
	
	printf("Duration: %lf\n", dur);
}

static void test_mallog(void)
{
	int i;
	void *ptrs[50] = { NULL };
	for (i = 0; i < 50; i++) {
		ptrs[i] = log_malloc(sizeof(int));
	}

	for (i = 0; i < 50; i++) {
		log_free(ptrs[i]);
	}

	log_dump();
	log_cleanup();
}

int main(int argc, char **argv)
{
	int i, found;
	found = 0;
	struct {
		char *a;
		char *argument;
		void (* run)(void);
	} lookup[] = {
		{"-a", "--arg", test_arg},
		{"-s", "--astar", test_astar},
		{"-d", "--debug", test_debug},
		{"-f", "--farray", test_farray},
		{"-g", "--gc", test_gc},
		{"-p", "--parray", test_parray},
		{"-y", "--py", test_py},
		{"-c", "--str", test_str},
		{"-t", "--timer", test_timer},
		{"-k", "--token", test_token},
		{"-o", "--tok", test_tok},
		{"-r", "--tree", test_tree},
		{"-i", "--io", test_io},
		{"-l", "--list", test_list},
		{"-m", "--mallog", test_mallog},
		{"-e", "--check", test_check},
		{NULL, NULL, NULL}
	};

	for (i = 0; lookup[i].argument; i++) {
		if (arg_check(argc, argv, lookup[i].a, lookup[i].argument)) {
			printf("Running: %s, %s\n", lookup[i].a, lookup[i].argument);
			lookup[i].run();
			found += 1;
		}
	}

	if (!found) {
		puts("Usage:");
		for (i = 0; lookup[i].argument; i++) {
			printf("%s, %s\n", lookup[i].a, lookup[i].argument);
		}
	}

	return 0;
}
