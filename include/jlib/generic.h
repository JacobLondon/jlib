#ifndef JLIB_GENERIC_H
#define JLIB_GENERIC_H

#define ref(Value) ((void*)&(Value))
#define val(Voidptr, Type) (*(Type *)(Voidptr))

#endif /* JLIB_GENERIC_H */