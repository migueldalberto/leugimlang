#ifndef EXPR_H
#define EXPR_H

#include "token.h"

#include <stdbool.h>

typedef struct expr expr_t;
typedef struct expr_list expr_list_t;

struct expr {
	enum {
		EXPR_UNDEFINED,
		EXPR_IDENTIFIER,
		EXPR_INTEGER_LITERAL,
		EXPR_STRING_LITERAL,
		EXPR_BOOLEAN_LITERAL,
		EXPR_UNARY,
		EXPR_ASSIGNMENT,
		EXPR_CALL,
		EXPR_BINARY
	} tag;
	union {
		struct EXPR_IDENTIFIER { char *value; } expr_identifier;
		struct EXPR_INTEGER_LITERAL { int *value; } expr_integer_literal;
		struct EXPR_STRING_LITERAL { char *value; } expr_string_literal;
		struct EXPR_BOOLEAN_LITERAL { bool* value; } expr_boolean_literal;
		struct EXPR_UNARY { token_t *operator; expr_t *right; } expr_unary;
		struct EXPR_ASSIGNMENT { token_t *operator; char *identifier; expr_t *right; } expr_assignment;
		struct EXPR_CALL { expr_t *callee; expr_list_t *args; } expr_call;
		struct EXPR_BINARY { token_t *operator; expr_t *left; expr_t *right; } expr_binary;
	} data;
};

struct expr_list {
	int capacity;
	int length;
	expr_t *exprs;
};

void init_expr_list(expr_list_t *list);
void expr_list_add(expr_list_t *list, expr_t expr);

void expr_simple_print(expr_t *expr);
void init_expr_integer_literal(expr_t *expr, int *value);
void init_expr_unary(expr_t *expr, token_t *operator, expr_t *right);
void init_expr_binary(expr_t *expr, token_t *operator, expr_t *left, expr_t *right);

#endif
