#include <stdio.h>
#include "vector_test.h"
#include "../include/vector.h"

#define ref(T, pointer) *((T *)pointer)

void vector_test()
{
    vector(int) *myvec = vector_init(sizeof(int));
    ref(int, vector_at(myvec, 0)) = 10;
    printf("%d\n", ref(int, vector_at(myvec, 0)));

    delete(myvec);
}
