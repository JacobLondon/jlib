#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "alloc.h"
#include "generic.h"
#include "map.h"

/**
 * node
 */

Node *node_init(char *key, void *value)
{
    Node *self;
    try_malloc(self, sizeof(struct node_s), "Could not malloc for node initialization");
    try_malloc(self->key, (1 + strlen(key)) * sizeof(char), "Could not malloc map key");
    strcpy(self->key, key);
    self->value = value;
    
    return self;
}

void node_free(Node *self)
{
    dealloc(self->key);
    dealloc(self);
}

/**
 * map
 */

Map *map_new(void (* f_free)(void *buf))
{
    Map *self;
    try_malloc(self, sizeof(struct map_s), "Could not malloc for map initialization");
    self->buckets = array_new(array_free);
    self->cap = MAP_DEFAULT_CAP;
    self->size = 0;
    self->free = f_free;

    // array to track an array of array pointers
    for (int i = 0; i < MAP_DEFAULT_CAP; i++)
        bucket_at(self, i) = NULL;

    return self;
}

void map_insert_(Map *self, Node *n)
{
    size_t index = fnv1a(n->key, self->cap);

    // the bucket hasn't been created yet
    if (bucket_at(self, index) == NULL)
        bucket_at(self, index) = array_new(self->free);
    // bucket created already, check if the key is already in it
    else {
        // check if the key is in the bucket
        for (size_t i = 0; i < bucket_at(self, index)->size; i++) {
            // the key exists, replace it with the new node and exit
            if (strcmp(n->key, node_at(self, index, i)->key) == 0) {
                node_free(node_at(self, index, i));
                node_at(self, index, i) = n;
                return;
            }
        }
    }

    // append the node, a bucket must be there
    array_push(bucket_at(self, index), n);
    
    // track the inserted item
    self->size++;

    // check the capacity to size ratio
    if (self->size >= self->cap * MAP_DEFAULT_MAX) {
        // double the size when the ratio is surpassed
        map_resize(self, self->cap * MAP_DEFAULT_SCALING);
    }
}

Node *map_at(Map *self, char *key)
{
    size_t index = fnv1a(key, self->cap);

    // look for the key in the bucket vector
    for (size_t i = 0; i < bucket_at(self, index)->size; i++) {
        if (bucket_at(self, index) == NULL)
            continue;
        if (strcmp(node_at(self, index, i)->key, key) == 0)
            return node_at(self, index, i);
    }

    // key not found
    return NULL;
}

void map_resize(Map *self, size_t size)
{
    // temporarily hold the nodes
    Array *temp = array_new((void (*)(void *))node_free);
    size_t i, j;

    // traverse the map
    for (i = 0; i < self->buckets->size; i++) {
        // the bucket is empty
        if (bucket_at(self, i) == NULL)
            continue;

        // traverse the bucket, record nodes
        for (j = 0; j < bucket_at(self, i)->size; j++) {
            array_push(temp, node_at(self, i, j));
        }
        
        // free the bucket, nodes stay in temp
        array_free(bucket_at(self, i));
    }

    // reallocate the buckets
    array_resize(self->buckets, size);
    self->cap = size;

    // fill with NULL
    for (i = 0; i < self->cap; i++)
        bucket_at(self, i) = NULL;

    // fill self from temp
    for (i = 0; i < temp->size; i++) {
        // correct for capacity++ in insert
        self->size--;
        map_insert_(self, ((Node **)temp->buf)[i]);
    }

    array_free(temp);
}

void map_free(Map *self)
{
    if (self == NULL)
        return;

    // traverse the map
    for (size_t i = 0; i < self->cap; i++) {
        // the bucket is empty
        if (bucket_at(self, i) == NULL)
            continue;
        // traverse the bucket, free every node
        for (size_t j = 0; j < bucket_at(self, i)->size; j++) {
            node_free(node_at(self, i, j));
        }
        // free each bucket
        array_free(bucket_at(self, i));
    }
    // free the map array and the map
    array_free(self->buckets);
    free(self);
}

/**
 * hash
 */

// https://create.stephan-brumme.com/fnv-hash/
const size_t Prime = 0x01000193; //   16777619
const size_t Seed  = 0x811C9DC5; // 2166136261

/**
 * @max MUST BE A POWER OF 2
 */
size_t fnv1a(const char *text, size_t max)
{
    size_t hash = Seed;
    while (*text)
        hash = (*text++ ^ hash) * Prime;
    
    // 64 bit number into the range defined by max
    hash = hash % max;
    return hash;
}
