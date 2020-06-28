#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <jlib/mallog.h>
#include <jlib/parray.h>

/* takes no ownership of anything */
struct info {
	void *ptr;
	size_t size;
	const char *sizeinfo;
	const char *alloc_file;
	const char *alloc_func;
	const char *alloc_line;
	const char *dealloc_file;
	const char *dealloc_func;
	const char *dealloc_line;
};

static struct parray *infos = NULL;

static struct info *info_new(void *ptr, size_t size, const char *sizeinfo);
static void info_log_alloc(struct info *self, const char *file, const char *func, const char *line);
static void info_log_dealloc(struct info *self, const char *file, const char *func, const char *line);
static void info_free(struct info *self);
static void info_dump(struct info *self);

void log_init(void)
{
	if (infos) {
		return;
	}

	infos = parray_new(info_free);
}

void log_cleanup(void)
{
	parray_free(infos);
	infos = NULL;
}

void log_dump(void)
{
	size_t i;

	if (!infos) {
		return;
	}

	for (i = 0; i < infos->size; i++) {
		info_dump(infos->buf[i]);
	}
}

void *_log_malloc(size_t size, const char *sizeinfo, const char *file, const char *func, const char *line)
{
	/* internally skips if already initialized */
	log_init();
	void *ptr = malloc(size);
	assert(ptr);
	struct info *in = info_new(ptr, size, sizeinfo);
	assert(in);
	
	info_log_alloc(in, file, func, line);
	parray_push_(infos, in);
	
	return ptr;
}

void *_log_calloc(size_t nmemb, size_t size, const char *sizeinfo, const char *file, const char *func, const char *line)
{
	/* internally skips if already initialized */
	log_init();
	void *ptr = calloc(nmemb, size);
	assert(ptr);
	struct info *in = info_new(ptr, size, sizeinfo);
	assert(in);

	info_log_alloc(in, file, func, line);
	parray_push_(infos, in);

	return ptr;
}

void *_log_realloc(void *ptr, size_t size, const char *sizeinfo, const char *file, const char *func, const char *line)
{
	/* internally skips if already initialized */
	log_init();
	/* Assume moved, so it can be freed and made new elsewhere,
	 * even if that isn't what is chosen to occur */
	_log_free(ptr, file, func, line);
	void *nptr = realloc(ptr, size);
	assert(ptr);
	struct info *in = info_new(nptr, size, sizeinfo);
	assert(in);

	info_log_alloc(in, file, func, line);
	parray_push_(infos, in);

	return ptr;

}

void *_log_strdup(const char *s, const char *sizeinfo, const char *file, const char *func, const char *line)
{
	/* internally skips if already initialized */
	log_init();
	void *ptr = strdup(s);
	assert(ptr);
	struct info *in = info_new(ptr, strlen(s), sizeinfo);
	assert(in);

	info_log_alloc(in, file, func, line);
	parray_push_(infos, in);

	return ptr;
}

void _log_free(void *ptr, const char *file, const char *func, const char *line)
{
	size_t i;
	struct info *in;
	/* internally skips if already initialized */
	log_init();
	for (i = 0; i < infos->size; i++) {
		in = (struct info *)infos->buf[i];
		if (in->ptr == ptr) {
			in->dealloc_file = file;
			in->dealloc_func = func;
			in->dealloc_line = line;
			break;
		}
	}
}

static struct info *info_new(void *ptr, size_t size, const char *sizeinfo)
{
	struct info *self = calloc(1, sizeof(struct info));
	if (!self) {
		return NULL;
	}
	self->ptr = ptr;
	self->size = size;
	self->sizeinfo = sizeinfo;
	return self;
}

static void info_log_alloc(struct info *self, const char *file, const char *func, const char *line)
{
	if (!self) {
		return;
	}
	self->alloc_file = file;
	self->alloc_func = func;
	self->alloc_line = line;
}

static void info_log_dealloc(struct info *self, const char *file, const char *func, const char *line)
{
	if (!self) {
		return;
	}
	self->dealloc_file = file;
	self->dealloc_func = func;
	self->dealloc_line = line;
}

static void info_free(struct info *self)
{
	if (!self) {
		return;
	}
	if (self->ptr) {
		free(self->ptr);
	}
	memset(self, 0, sizeof(struct info));
	free(self);
}

static void info_dump(struct info *self)
{
	if (!self) {
		return;
	}

	printf("%p: %s == %zu bytes | alloc'ed %s:%s:%s | free'd %s:%s:%s\n",
		self->ptr, self->sizeinfo, self->size,
		self->alloc_file, self->alloc_func, self->alloc_line,
		self->dealloc_file, self->dealloc_func, self->dealloc_line);
}
