TARGET=jlibtest
CC=clang
CFLAGS=-ggdb -std=c11 -O2

OBJ_FILES=include/allocator.o include/array.o include/debug.o include/print.o \
	include/test.o

.PHONY: clean

$(TARGET): $(OBJ_FILES)
	$(CC) -o $(TARGET) $(OBJ_FILES) $(CFLAGS)

clean:
	rm -rf $(TARGET) $(OBJ_FILES)
