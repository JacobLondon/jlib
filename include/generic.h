#ifndef JLIB_GENERIC_H
#define JLIB_GENERIC_H

#include <stdlib.h>

void *copy(void *buf, size_t size);
void delete(void *buf, void (*f_free)(void *buf));

#define ref(Value) ((void*)&(Value))
#define val(Voidptr, Type) (*(Type *)(Voidptr))

#endif // JLIB_GENERIC_H