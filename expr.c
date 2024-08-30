#include "expr.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_with_indent(expr_t *expr, int indent_number) {
	char *indent = (char *) malloc(2 * indent_number * sizeof(char) + 1);
	indent[0] = '\0';
	for (int i = 0; i < indent_number; ++i) {
		strcat(indent, "  ");
	}

	if (expr == NULL) {
		printf("%s?\n", indent);
		return;
	}

	switch (expr->tag) {
		case EXPR_UNDEFINED:
			printf("%sundefined\n", indent);
			break;
		case EXPR_IDENTIFIER:
			printf("%s%s\n", indent, expr->data.expr_identifier.value);
			break;
		case EXPR_STRING_LITERAL:
			printf("%s%s\n", indent, expr->data.expr_string_literal.value);
			break;
		case EXPR_INTEGER_LITERAL:
			printf("%s%d\n", indent, *expr->data.expr_integer_literal.value);
			break;
		case EXPR_BOOLEAN_LITERAL:
			printf("%s%d\n", indent, *expr->data.expr_boolean_literal.value);
			break;
		case EXPR_UNARY:
			printf("%s%s\n", indent, expr->data.expr_unary.operator->lexeme);
			print_with_indent(expr->data.expr_unary.right, indent_number + 1);
			break;
		case EXPR_ASSIGNMENT:
			printf("%s%s %s\n", indent, expr->data.expr_assignment.identifier, expr->data.expr_assignment.operator->lexeme);
			print_with_indent(expr->data.expr_assignment.right, indent_number + 1);
			break;
		case EXPR_BINARY:
			printf("%s%s\n", indent, expr->data.expr_binary.operator->lexeme);
			print_with_indent(expr->data.expr_binary.left, indent_number + 1);
			print_with_indent(expr->data.expr_binary.right, indent_number + 1);
			break;
	};

	free(indent);
}

void expr_simple_print(expr_t *expr) {
	print_with_indent(expr, 0);
}

void init_expr_integer_literal(expr_t *expr, int *value) {
	expr->tag = EXPR_INTEGER_LITERAL;
	expr->data.expr_integer_literal.value = value;
}

void init_expr_binary(expr_t *expr, token_t *operator, expr_t *left, expr_t *right) {
	expr->tag = EXPR_BINARY;
	expr->data.expr_binary.operator = operator;
	expr->data.expr_binary.left = left;
	expr->data.expr_binary.right = right;
}

void init_expr_unary(expr_t *expr, token_t *operator, expr_t *right) {
	expr->tag = EXPR_UNARY;
	expr->data.expr_unary.operator = operator;
	expr->data.expr_unary.right = right;
}
