TARGET=jlibtest
CC=clang
CFLAGS=-ggdb

OBJ_FILES=include/allocator.o include/debug.o include/print.o \
	test/allocator_test.o test/print_test.o test/test.o

.PHONY: clean

$(TARGET): $(OBJ_FILES)
	$(CC) -o $(TARGET) $(OBJ_FILES) $(CFLAGS)

clean:
	rm -rf $(TARGET) $(OBJ_FILES)
