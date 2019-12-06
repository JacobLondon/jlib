#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "debug.h"
#include "generic.h"
#include "map.h"

/**
 * node
 */

Node node_init(char *key, void *value)
{
    Node self;
    if (!(self = malloc(sizeof(struct node_s))))
        halt("Could not malloc for node initialization");
    if (!(self->key = malloc((1 + strlen(key)) * sizeof(char))))
        halt("Could not malloc map key");
    strcpy(self->key, key);
    self->value = value;
    
    return self;
}

void node_free(Node self)
{
    delete(self->key, free);
    delete(self, free);
}

/**
 * map
 */

Map map_new(void (* f_free)(void *buf))
{
    Map self;
    if (!(self = malloc(sizeof(struct map_s))))
        halt("Could not malloc for map initialization");
    self->buckets = array_new(node_free);
    self->cap = MAP_DEFAULT_CAP;
    self->size = 0;
    self->free = f_free;

    // vector to track an array of vector pointers
    for (int i = 0; i < MAP_DEFAULT_CAP; i++)
        bucket_at(self, i) = NULL;

    return self;
}

void map_insert(Map self, Node n)
{
    size_t index = fnv1a(n->key, self->cap);

    // the bucket hasn't been created yet
    if (bucket_at(self, index) == NULL)
        bucket_at(self, index) = array_new(self->free);
    
    // check if the key is in the bucket
    for (size_t i = 0; i < bucket_at(self, index)->size; i++) {
        // the key exists, replace it with the new node and exit
        if (strcmp(n->key, node_at(self, index, i)->key) == 0) {
            node_free(node_at(self, index, i));
            node_at(self, index, i) = n;
            return;
        }
    }
    // else

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

Node map_at(Map self, char *key)
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

void map_resize(Map self, size_t size)
{
    // temporarily hold the nodes
    Array temp = array_new(self->free);
    size_t i, j;

    // traverse the map
    for (i = 0; i < self->buckets->size; i++) {
        // the bucket is empty
        if (bucket_at(self, i) == NULL)
            continue;

        // traverse the bucket, record nodes
        for (j = 0; j < bucket_at(self, i)->size; j++) {
            vector_append(temp, node_at(self, i, j));
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
        map_insert(self, ((Node *)temp->buf)[i]);
    }

    array_free(temp);
}

void map_free(Map self)
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

/**
 * test
 */

static char *rand_string(char *str, size_t size)
{
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJK";
    if (size) {
        --size;
        for (size_t n = 0; n < size; n++) {
            int key = rand() % (int) (sizeof charset - 1);
            str[n] = charset[key];
        }
        str[size] = '\0';
    }
    return str;
}

static map_test()
//void main()
{
    Map m = map_init(NULL);
    char a = 'a';

    map_insert(m, node_init("aaaa", pointer_to(a)));
    map_insert(m, node_init("aa2a", pointer_to(a)));
    map_insert(m, node_init("aa12a", pointer_to(a)));
    map_insert(m, node_init("aaaa", pointer_to(a)));
    map_insert(m, node_init("a43aaa", pointer_to(a)));
    map_insert(m, node_init("aaaa", pointer_to(a)));
    map_insert(m, node_init("aafasdaa", pointer_to(a)));
    map_insert(m, node_init("aaaa", pointer_to(a)));
    map_insert(m, node_init("aaadsaa", pointer_to(a)));
    map_insert(m, node_init("afaaa", pointer_to(a)));
    map_insert(m, node_init("adaadaaa", pointer_to(a)));

    printf("capacity: %d\nsize: %d\n", m->cap, m->size);

    Node n;
    map_for_each(m, n) {
        printf("Bucket %d:", __b);
        node_print(n);
    }

    printf("aaaa: ");
    printf("%d\n", val(map_at(m, "aaaa")->value, char));

    map_free(m);
}
