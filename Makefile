TARGET=jlibtest
CC=clang
CFLAGS=-std=c11 -O2

OBJ_FILES=include/alloc.o include/arg.o include/array.o include/debug.o \
		  include/map.o include/stringify.o

.PHONY: clean

$(TARGET): test.o jlib.a
	$(CC) -o $@ $^ $(CFLAGS)

test.o: include/test.c
	$(CC) -c $< -o $@ $(CFLAGS)

jlib.a: $(OBJ_FILES)
	ar rcs $@ $^

static:


clean:
	rm -rf $(TARGET) $(OBJ_FILES) *.a *.o
