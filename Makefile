TARGET=jlibtest
CC=gcc
CFLAGS = -std=c11 -O2 -Wall -Wextra -lm -Iinclude/

OBJ_FILES=\
	src/arg.o \
	src/astar.o \
	src/debug.o \
	src/farray.o \
	src/fmap.o \
	src/gc.o \
	src/io.o \
	src/list.o \
	src/mallog.o \
	src/math.o \
	src/parray.o \
	src/str.o \
	src/timer.o \
	src/util.o

.PHONY: clean

$(TARGET): test.o jlib.a
	$(CC) -o $@ $^ $(CFLAGS)

test.o: src/test.c
	$(CC) -c $< -o $@ $(CFLAGS)

jlib.a: $(OBJ_FILES)
	ar rcs $@ $^

clean:
	rm -rf $(TARGET) $(OBJ_FILES) *.a *.o
