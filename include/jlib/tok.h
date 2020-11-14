#ifndef JLIB_TOK_H
#define JLIB_TOK_H

#include <stddef.h>

#include <jlib/farray.h>

struct tokenizer;

/**
 * Return the next token using tokenizer_next to get next characters.
 * 
 * \warning
 *   Ensure an EOF token's symbol is NULL
 */
typedef struct token (* tokenizer_gettok_func)(struct tokenizer *ctx);

struct token {
	char *symbol;
	int len;
	int id;
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

/**
 * Reset the tokenizer's state. Must be done before tokenizing a second time
 */
void tokenizer_reset(struct tokenizer *self);

/**
 * Get the next token, symbol is NULL @ EOF
 */
struct token tokenizer_gettok(struct tokenizer *self);

/**
 * Get the next char, 0 on EOF
 */
char tokenizer_next(struct tokenizer *self);

/**
 * Get the number of tokens in the buffer
 */
size_t tokenizer_count(struct tokenizer *self);

#endif // JLIB_TOK_H
