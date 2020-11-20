TARGET=jlibtest
CC=gcc
CFLAGS = -std=c11 -Wall -Wextra -lm -Iinclude/ -ggdb

OBJ_FILES=\
	src/arg.o \
	src/astar.o \
	src/debug.o \
	src/func.o \
	src/farray.o \
	src/fmap.o \
	src/gc.o \
	src/io.o \
	src/list.o \
	src/mallog.o \
	src/math.o \
	src/parray.o \
	src/str.o \
	src/tok.o \
	src/timer.o \
	src/tree.o \
	src/util.o

FILES=$(wildcard src/*.c)

.PHONY: clean

$(TARGET): test.o jlib.a
	$(CC) -o $@ $^ $(CFLAGS)

test.o: test/test.c
	$(CC) -c $< -o $@ $(CFLAGS)

jlib.a: $(OBJ_FILES)
	ar rcs $@ $^

parse.o: test/parse.c
	$(CC) -c $< -o $@ $(CFLAGS)

parse: parse.o jlib.a
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	rm -rf $(TARGET) $(OBJ_FILES) *.a *.o

tcc:
	tcc -o $(TARGET) $(FILES) test/parse.c -Iinclude/ -Wall -Wextra -ggdb
