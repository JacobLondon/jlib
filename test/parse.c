#include <assert.h>
#include <ctype.h>

#include <jlib/jlib.h>

enum symbols {
	ID_NONE,
	ID_INTEGER,
	ID_LPAREN,
	ID_RPAREN,
	ID_ADD,
	ID_MULT,
	ID_END,
	ID_UNKN,

	ID_EXPR,
	ID_TERM,
	ID_NUMBER
};

// nonterminal instances
static struct token symbol_expr = (struct token){ .symbol=NULL, .len=0, .id=ID_EXPR };
static struct token symbol_term = (struct token){ .symbol=NULL, .len=0, .id=ID_TERM };
static struct token symbol_number = (struct token){ .symbol=NULL, .len=0, .id=ID_NUMBER };
static struct token *symptr = NULL;

static struct token parse_gettok(struct tokenizer *ctx);

/**
 * Expr
 *   Term + Term
 *   Term - Term
 * Term
 *   NUM * NUM
 *   NUM / NUM
 * NUM
 *  [1-9][0-9]*
 *  ( Expr )
 */

static void parse_expr(struct tree_node *current);
static void parse_term(struct tree_node *current);
static void parse_number(struct tree_node *current);

static void parse_expr(struct tree_node *current)
{
	struct tree_node *tmp;

	assert(current);

	tmp = tree_node_append(current, &symbol_term);
	parse_term(tmp);

	if (symptr[0].id != ID_ADD) {
		printf("Error\n");
		exit(1);
	}

	tree_node_append(current, symptr);
	symptr++;

	tmp = tree_node_append(current, &symbol_term);
	parse_term(tmp);
}

static void parse_term(struct tree_node *current)
{
	assert(current);

}

static void parse_number(struct tree_node *current)
{
	assert(current);

}

static struct token parse_gettok(struct tokenizer *ctx)
{
	struct token rv;
	char ch;

	// skip whitespace
	while (isspace(ctx->cursor[0])) {
		(void)tokenizer_next(ctx);
	}

	if (isdigit(ctx->cursor[0])) {
		rv.symbol = ctx->cursor;
		rv.id = ID_INTEGER;
		for (rv.len = 1; (ch = tokenizer_next(ctx)) && isdigit(ch); rv.len++);
	}
	else if (ctx->cursor[0] == '+') {
		rv.symbol = ctx->cursor;
		rv.id = ID_ADD;
		rv.len = 1;
		(void)tokenizer_next(ctx);
	}
	else if (ctx->cursor[0] == '*') {
		rv.symbol = ctx->cursor;
		rv.id = ID_MULT;
		rv.len = 1;
		(void)tokenizer_next(ctx);
	}
	else if (ctx->cursor[0] == '(') {
		rv.symbol = ctx->cursor;
		rv.id = ID_LPAREN;
		rv.len = 1;
		(void)tokenizer_next(ctx);
	}
	else if (ctx->cursor[0] == ')') {
		rv.symbol = ctx->cursor;
		rv.id = ID_RPAREN;
		rv.len = 1;
		(void)tokenizer_next(ctx);
	}
	else if (ctx->cursor[0] == 0) {
		rv.symbol = NULL;
		rv.id = ID_END;
		rv.len = 1;
	}
	else {
		rv.symbol = ctx->cursor;
		rv.id = ID_UNKN;
		rv.len = 1;
		(void)tokenizer_next(ctx);
	}

	return rv;
}

int main(void)
{
	size_t i;
	char *program = "5 + 1";

	// tokenize
	struct token tokens[256];
	struct tokenizer tokenizer;

	(void)memset(tokens, 0, sizeof(tokens));
	tokenizer_new(program, strlen(program), parse_gettok, &tokenizer);

	puts("===== Tokens =====");
	for (tokens[i = 0] = tokenizer_gettok(&tokenizer);
		 tokens[i].id != ID_END;
		 tokens[++i] = tokenizer_gettok(&tokenizer))
	{
		token_puts(&tokens[i]);
	}
	for (i = 0; tokens[i].id != ID_END; i++) {
		if (tokens[i].id == ID_UNKN) {
			printf("Error: Unknown tok: ");
			token_puts(&tokens[i]);
		}
	}

	puts("===== Parse =====");
	struct tree_node *root = tree_node_new(&symbol_expr);
	parse_expr(root);

	return 0;
}
