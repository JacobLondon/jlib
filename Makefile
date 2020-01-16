TARGET=jlibtest
CC=gcc
CFLAGS=-std=c89 -O2 -Iinclude/ -Wall -Wextra

OBJ_FILES=src/arg.o src/debug.o src/farray.o src/io.o src/math.o \
		  src/parray.o src/str.o src/timer.o src/util.o

.PHONY: clean

$(TARGET): test.o jlib.a
	$(CC) -o $@ $^ $(CFLAGS)

test.o: src/test.c
	$(CC) -c $< -o $@ $(CFLAGS)

jlib.a: $(OBJ_FILES)
	ar rcs $@ $^

clean:
	rm -rf $(TARGET) $(OBJ_FILES) *.a *.o
