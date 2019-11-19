#ifndef JLIB_GENERIC_H
#define JLIB_GENERIC_H

#define ref(value) ((void*)&(value))
#define val(voidptr, type) (*(type *)(voidptr))

#endif // JLIB_GENERIC_H