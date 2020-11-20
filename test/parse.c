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
static struct token symbol_expr = { .symbol="expr", .len=4, .id=ID_EXPR };
static struct token symbol_term = { .symbol="term", .len=4, .id=ID_TERM };
static struct token symbol_number = { .symbol="num", .len=3, .id=ID_NUMBER };
static struct token *symptr = NULL;
static int terminal = ID_END;

static struct token parse_gettok(struct tokenizer *ctx);

/**
 * TODO
 * Program
 *   Expr Expr
 *   Expr
 * Expr
 *   Term + Term
 *   Term - Term
 *   Term
 * Term
 *   NUM * NUM
 *   NUM / NUM
 *   NUM
 * NUM
 *  [1-9][0-9]*
 *  ( Expr )
 */

static struct token *parse_peek(void);
static void parse_expr(struct tree_node *current);
static void parse_term(struct tree_node *current);
static void parse_number(struct tree_node *current);
static void parse_print_tree(void *value, size_t depth);

static struct token *parse_peek(void)
{
	if (!symptr) {
		return NULL;
	}

	if (symptr->id == terminal) {
		return NULL;
	}

	return &symptr[1];
}

static void parse_expr(struct tree_node *current)
{
	static struct tree_node *tmp;
	assert(current);

	tmp = tree_node_append(current, &symbol_term);
	parse_term(tmp);

	// standalone TERM
	if (symptr[0].id != ID_ADD) {
		return;
	}

	tree_node_append(current, symptr);
	symptr++;

	tmp = tree_node_append(current, &symbol_term);
	parse_term(tmp);
}

static void parse_term(struct tree_node *current)
{
	static struct tree_node *tmp;
	assert(current);

	tmp = tree_node_append(current, &symbol_number);
	parse_number(tmp);

	// standalone NUM
	if (symptr[0].id != ID_MULT) {
		return;
	}

	tree_node_append(current, symptr);
	symptr++;

	tmp = tree_node_append(current, &symbol_number);
	parse_number(tmp);
}

static void parse_number(struct tree_node *current)
{
	static struct tree_node *tmp;
	assert(current);

	switch (symptr[0].id) {
	case ID_LPAREN:
		symptr++;
		tmp = tree_node_append(current, &symbol_expr);
		parse_expr(tmp);
		if (symptr[0].id != ID_RPAREN) {
			printf("Missing RPAREN: ");
			token_puts(symptr);
			exit(1);
		}
		break;
	case ID_INTEGER:
		tmp = tree_node_append(current, symptr);
		symptr++;
		break;
	default:
		printf("Unexpected token: ");
		token_puts(symptr);
		exit(1);
	}
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

static void parse_print_tree(void *value, size_t depth)
{
	struct token *tok = (struct token *)value;
	for ( ; depth > 0; depth--) {
		printf("  ");
	}
	token_puts(tok);
}

int main(void)
{
	size_t i;
	//char *program = "5 + 1";
	//char *program = "5 + (1 * 1)";
	char *program = "5 + 2 * 3 + 10";

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
	symptr = tokens;
	parse_expr(root);

	tree_node_map(root, parse_print_tree);

	return 0;
}
