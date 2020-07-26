# J Lib
C Library with utility functions. Some functions from the CPython and Linux projects.

# Dependencies
```
# Clang
sudo apt install clang
# GNU Make
sudo apt install make
# Python
sudo apt-get install python3-dev
pip3 install matplotlib
```

# Building
Run `make`. The resulting binary will be `jlibtest`, and the resulting static library file will be `jlib.a`.

# Features
```C
// arg.c
// 1 if the arg is present, else 0
int arg_check(int argc, char **argv, const char *arg);
// return the next arg if the arg is present, else NULL
char *arg_get(int argc, char **argv, const char *arg)

// astar.c
// initialize the A* pathfinding algorithm
void astar_init(size_t width, size_t height, bool (*check_obstacle_cb)(int i, int j));
// clean up the A* pathfinding algorithm
void astar_cleanup(void);
// perform a pathfinding, see astar.h for more info
size_t astar_path(unsigned *xs, unsigned *ys, size_t size, unsigned si, unsigned sj, unsigned ei, unsigned ej);

// draw.c
// draw a triangle with scan-line rasterization
void draw_triangle(Color c, int x1, int y1, int x2, int y2, int x3, int y3)

// farray.c
// A flat array, does not require pointers to be passed

// fmap.c
// A flat hash table (associative array / dict), see fmap.h

// gc.c
// Perform allocation with a mark and sweep garbage collector
struct gc *collector = gc_new();
int *array0 = gc_alloc_dtor(collector, 10, NULL);
// mark the sweep point
gc_mark(collector);
int *array1 = gc_alloc_dtor(collector, 20, NULL);
// take an owned pointer
int *array2 = malloc(sizeof(int) * 30);
gc_push_dtor(collector, array2);
// sweep until only array0 is left
gc_collect(collector);
// sweep array0
gc_collect(collector);

// io.c
// See io.h

// mallog.c
// Malloc functions which perform a stdout logging upon log_dump()
void *log_malloc(size_t size);
void *log_calloc(size_t nmemb, size_t size);
void *log_realloc(void *ptr, size_t size);
char *log_strdup(const char *str);
void log_free(void *ptr);

// math.c
// Misc math functions (including fast sqrt / sin / cos)

// parray.c
// A dynamic array of pointers with auto deallocation util
struct parray *array = parray_new(free);
int *data = malloc(sizeof(int));
parray_push(array, data);
parray_free(parray);

// str.c
// super useful string concatenation with some format specifiers
char *str = NULL;
strcatf(&str, "This is one hundred: %d", 100);
printf("%s\n", str); // This is one hundred: 100
free(str);
// strings must be heap allocated
char *builder = strdup("Lorem");
strcatf(&builder, "%s", " Ipsum");
printf("%s\n", builder); // Lorem Ipsum
free(str);
// split strings by any of the characters in the format arg
char *str = strdup("100, 10|60;100");
char **buf = strsplit(str, ", |;");
for (int i = 0; buf[i]; i++) {
	printf("%s\n", buf[i]);
}
/**
 * Prints:
 * 100
 * 10
 * 60
 * 100
 */
strsplit_free(buf);
free(str);

// replace all instances of a substring within a string
char *str = strdup("file.txt something.txt else.txt");
streplace(&str, ".txt", "");
printf("%s\n", str); // file something else

// timer.c
timer_rst();
// ...
// long op
// ...
printf("Duration: %lf\n", timer_lap());
```
