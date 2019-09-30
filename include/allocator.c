#include <stdio.h>
#include <stdlib.h>
#include "allocator.h"

allocator *allocator_init(int (* new)(void *owner), int (*del)(void *owner))
{
    allocator *self = malloc(sizeof(allocator));
    self->new = new;
    self->del = del;
    return self;
}

void allocator_free(allocator *self)
{
    if (!self)
        return;

    self->new = NULL;
    self->del = NULL;
    free(self);
    self = NULL;
}

/**
 * test
 */

struct data {
    allocator *alloc;
    int *d;
};

static int data_new(void *buf)
{
    struct data *self = (struct data *)buf;
    self->d = calloc(8, sizeof(int));
    return 1;
}

static int data_del(void *buf)
{
    struct data *self = (struct data *)buf;
    for (int i = 0; i < 8; i++)
        self->d[i] = 0;
    
    free(self->d);
    self->d = NULL;
    allocator_free(self->alloc);
    free(buf);
    buf = NULL;
    
    return 1;
}

static struct data *data_init()
{
    struct data *self = malloc(sizeof(struct data));
    self->alloc = allocator_init(data_new, data_del);
    allocate(self);
    return self;
}

void allocator_test()
{
    struct data *d = data_init();
    lifetime (d) {
        d->d[0] = 10;
        for (int i = 0; i < 8; i++)
            printf("%d\n", d->d[i]);
    }

}
