#ifndef STMT_H
#define STMT_H

#include "token.h"
#include "expr.h"

#include <stdbool.h>
#include <stdlib.h>

typedef struct stmt stmt_t;

typedef struct {
	int capacity;
	int length;
	stmt_t *stmts;
} stmt_list_t;

struct stmt {
	enum {
		STMT_PRINT,
		STMT_WHILE,
		STMT_EXPRESSION,
		STMT_DECLARATION
	} tag;
	union {
		struct STMT_PRINT { expr_t *expr; } stmt_print;
		struct STMT_WHILE { expr_t *condition; stmt_list_t *stmt_list; } stmt_while;
		struct STMT_EXPRESSION { expr_t *expr; } stmt_expression;
		struct STMT_DECLARATION { token_t *identifier; expr_t *expr; } stmt_declaration;
	} data;
};

void init_stmt_list(stmt_list_t *list);
void stmt_list_add(stmt_list_t *list, stmt_t stmt);
void stmt_simple_print(stmt_t *stmt);

#endif
