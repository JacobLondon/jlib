#ifndef JLIB_GENERIC_H
#define JLIB_GENERIC_H

#include <stdlib.h>

#define jlib_ref(Value) ((void*)&(Value))
#define jlib_val(Voidptr, Type) (*(Type *)(Voidptr))

#endif // JLIB_GENERIC_H