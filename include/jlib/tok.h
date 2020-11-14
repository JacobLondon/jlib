#ifndef JLIB_TOK_H
#define JLIB_TOK_H

#include <stddef.h>

#include <jlib/farray.h>

typedef struct token (* tokenizer_gettok_func)(struct tokenizer *ctx);

struct token {
	char *symbol;
	size_t len;
	size_t id;
};

void token_puts(struct token *self);

struct tokenizer {
	char *cursor; // current position
	char *front; // yes just read the whole file
	size_t lineno;
	size_t colno;
	size_t len;
	tokenizer_gettok_func gettok;
};

void tokenizer_new(char *text, size_t len, tokenizer_gettok_func gettok, struct tokenizer *out);
void tokenizer_del(struct tokenizer *self);
struct token tokenizer_gettok(struct tokenizer *self);
char tokenizer_next(struct tokenizer *self);

#endif // JLIB_TOK_H
