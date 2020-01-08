TARGET=jlibtest
CC=clang
CFLAGS=-std=c11 -O2 -Iinclude/

OBJ_FILES=src/alloc.o src/arg.o src/array.o src/debug.o \
		  src/map.o src/math.o src/stringify.o

.PHONY: clean

$(TARGET): test.o jlib.a
	$(CC) -o $@ $^ $(CFLAGS)

test.o: src/test.c
	$(CC) -c $< -o $@ $(CFLAGS)

jlib.a: $(OBJ_FILES)
	ar rcs $@ $^

clean:
	rm -rf $(TARGET) $(OBJ_FILES) *.a *.o
