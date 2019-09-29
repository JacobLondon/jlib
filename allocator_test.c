#include <stdlib.h>
#include <stdio.h>
#include "allocator.h"

struct data {
    allocator *alloc;
    int *d;
};

static int data_new(void *buf)
{
    struct data *self = (struct data *)(buf);
    self->d = calloc(8, sizeof(int));
    return 1;
}

static int data_del(void *buf)
{
    struct data *self = (struct data *)(buf);
    for (int i = 0; i < 8; i++)
        self->d[i] = 0;
    
    free(self->d);
    self->d = NULL;
    allocator_free(self->alloc);
    free(buf);
    buf = NULL;
    
    return 1;
}

static struct data *data_construct()
{
    struct data *self = malloc(sizeof(struct data));
    self->alloc = allocator_init(data_new, data_del);
    allocate(self);
    return self;
}

int main()
{
    struct data *d = data_construct();

    d->d[0] = 10;
    for (int i = 0; i < 8; i++)
        printf("%d\n", d->d[i]);

    delete(d);
}
