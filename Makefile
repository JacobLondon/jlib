TARGET=jlibtest
CC=clang
CFLAGS=-std=c11 -O2

OBJ_FILES=include/array.o include/debug.o include/generic.o \
		  include/map.o include/print.o include/stringify.o \
		  include/test.o

.PHONY: clean

$(TARGET): $(OBJ_FILES)
	$(CC) -o $(TARGET) $(OBJ_FILES) $(CFLAGS)

clean:
	rm -rf $(TARGET) $(OBJ_FILES)
