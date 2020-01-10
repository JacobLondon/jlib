#ifndef JLIB_ARRAY_H
#define JLIB_ARRAY_H

#include <stdlib.h> /* size_t */

#include "generic.h"

#define ARRAY_DEFAULT_CAP 8
#define ARRAY_DEFAULT_SCALING 2

struct array_s {
	void **buf;
	size_t size;
	size_t cap;
	void (* free)(void *buf);
};

struct array_s *array_new(void (* f_free)(void *buf));
void array_free(struct array_s *self);
void array_push_(struct array_s *self, void *value);
void array_pop(struct array_s *self);
void array_resize(struct array_s *self, size_t size);

#define array_push(Arrayptr, Valueptr) array_push_((Arrayptr), ref((Valueptr)))
#define array_read(Arrayptr, Index, Type) (val((Arrayptr)->buf[(Index)], Type))
#define array_write(Arrayptr, Index, Valueptr) do { (Arrayptr)->buf[(Index)] = ref((Valueptr)); } while (0)

#endif /* JLIB_ARRAY_H */