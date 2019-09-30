TARGET=jlibtest
CC=clang
CFLAGS=-ggdb

OBJ_FILES=include/allocator.o include/debug.o include/owned.o include/print.o include/vector.o \
	include/test.o

.PHONY: clean

$(TARGET): $(OBJ_FILES)
	$(CC) -o $(TARGET) $(OBJ_FILES) $(CFLAGS)

clean:
	rm -rf $(TARGET) $(OBJ_FILES)
