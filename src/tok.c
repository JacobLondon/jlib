#include <assert.h>
#include <memory.h>
#include <stdio.h>

#include <jlib/tok.h>

void token_puts(struct token *self)
{
	assert(self);
	(void)fprintf(stdout, "%d :: '%.*s'\n", self->id, self->len, self->symbol);
}

void tokenizer_new(char *text, size_t len, tokenizer_gettok_func gettok, struct tokenizer *out)
{
	assert(text);
	assert(gettok);
	assert(out);

	(void)memset(out, 0, sizeof(*out));
	out->cursor= text;
	out->front = text;
	out->lineno = 1;
	out->colno = 1;
	out->len = len;
	out->gettok = gettok;
}

void tokenizer_del(struct tokenizer *self)
{
	assert(self);
	(void)memset(self, 0, sizeof(*self));
}

void tokenizer_reset(struct tokenizer *self)
{
	assert(self);

	self->cursor = self->front;
	self->lineno = 1;
	self->colno = 1;
}

struct token tokenizer_gettok(struct tokenizer *self)
{
	assert(self);
	return self->gettok(self);
}

char tokenizer_next(struct tokenizer *self)
{
	assert(self);

	// no more
	if (self->cursor[0] == 0) {
		return 0;
	}
	// user says we have no more
	else if ((size_t)(self->cursor - self->front) >= (size_t)self->len) {
		return 0;
	}
	else if (self->cursor[1] == '\r') {
		self->colno = 1;
	}
	else if (self->cursor[1] == '\n') {
		self->colno = 1;
		self->lineno++;
	}
	self->cursor++;
	return self->cursor[0];
}

size_t tokenizer_count(struct tokenizer *self)
{
	size_t i = 0;
	struct token tok;

	assert(self);
	tokenizer_reset(self);

	for (tok = tokenizer_gettok(self);
	     tok.symbol != NULL;
	     tok = tokenizer_gettok(self))
	{
		i++;
	}

	tokenizer_reset(self);
	return i;
}
