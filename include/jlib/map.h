#if 0

#ifndef JLIB_MAP_H
#define JLIB_MAP_H

#include "array.h"
#include "generic.h"

/**
 * node
 */

struct node_s {
    char *key;
    void *value;
};

typedef struct node_s Node;

Node *node_init(char *key, void *value);
void node_free(Node *self);
void node_print(Node *self);

/**
 * map
 */


struct map_s {
    Array *buckets;
    size_t size;    // number of buckets with items
    size_t cap;     // number of buckets
    void (* free)(void *buf);
};

typedef struct map_s Map;

#define MAP_DEFAULT_CAP 8      /* must be power of 2 */
#define MAP_DEFAULT_SCALING 2
#define MAP_DEFAULT_MAX 0.67f

#define node_at(map, bindex, nindex) \
    ((Node **)bucket_at((map), (bindex))->buf)[(nindex)]

#define bucket_at(Mapptr, Index) \
    ((Array **)((Mapptr)->buckets->buf))[(Index)]

#define map_for_each(map, node) \
    for (size_t __b = 0; __b < (map)->size; __b++) \
        if (bucket_at((map), __b) == NULL)  {\
            continue; \
        } \
        else \
            for (size_t __n = 0; __n < bucket_at((map), __b)->size; __n++) \
                if ((node = node_at((map), __b, __n)) != NULL)

Map *map_new(void (* f_free)(void *buf));
void map_free(Map *self);
Node *map_at(Map *self, char *key);
void map_insert_(Map *self, Node *n);
void map_resize(Map *self, size_t size);

#define map_insert(Mapptr, Key, Value) map_insert_(Mapptr, node_init(Key, ref(Value)))

/**
 * hash
 */

size_t fnv1a(const char *text, size_t max);

#endif // JLIB_MAP_H

#endif