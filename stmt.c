#include "stmt.h"
#include "expr.h"

#include <stdio.h>

void stmt_simple_print(stmt_t *stmt) {
	switch (stmt->tag) {
		case STMT_PRINT:
			printf("print (\n");
			expr_simple_print(stmt->data.stmt_print.expr);
			printf(")\n");
			break;
		case STMT_WHILE:
			printf("while \ncondition (\n");
			expr_simple_print(stmt->data.stmt_while.condition);
			printf(")\n");
			printf("statements (\n");
			for (int i = 0; i < stmt->data.stmt_while.stmt_list->length; ++i)
				stmt_simple_print(&stmt->data.stmt_while.stmt_list->stmts[i]);
			printf(")\n");
			break;
		case STMT_EXPRESSION:
			printf("expression (\n");
			expr_simple_print(stmt->data.stmt_expression.expr);
			printf(")\n");
			break;
		case STMT_DECLARATION:
			printf("var %s = (\n", stmt->data.stmt_declaration.identifier->literal);
			expr_simple_print(stmt->data.stmt_declaration.expr);
			printf(")\n");
			break;
	}
}

void init_stmt_list(stmt_list_t *list) {
	list->capacity = 1;
	list->length = 0;
	list->stmts = (stmt_t *) malloc(sizeof(stmt_t));
}

void stmt_list_add(stmt_list_t *list, stmt_t stmt) {
	if (list->length + 1 >= list->capacity) {
		list->capacity *= 2;
		stmt_t *new_list = (stmt_t *) malloc(list->capacity * sizeof(stmt_t));

		for (int i = 0 ; i < list->length; ++i) {
			new_list[i] = list->stmts[i];
		}

		free(list->stmts);
		list->stmts = new_list;
	}

	list->stmts[list->length] = stmt;
	list->length += 1;
}

