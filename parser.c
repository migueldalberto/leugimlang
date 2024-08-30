#include "parser.h"
#include "token.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void parser_error(token_t *token, const char *msg) {
	fprintf(stderr, 
		"[PARSER] [at %d, %s] '%s'\n", 
		token->position,
		token_type_to_string(token->type),
		msg
		);
}

stmt_list_t *parse_tokens(token_list_t *token_list) {
	stmt_list_t *stmt_list = (stmt_list_t *) malloc(sizeof(stmt_list_t));
	init_stmt_list(stmt_list);

	int position = 0;
	
	while (token_list->tokens[position].type != EOF_TOKEN &&
		position < token_list->length) {
		assert(position < token_list->length);
		stmt_t *stmt = parse_statement(token_list, &position);
		stmt_list_add(stmt_list, *stmt);
	}

	return stmt_list;
}

stmt_t *parse_statement(token_list_t *token_list, int *position) {
	assert(*position < token_list->length);
	stmt_t *statement = (stmt_t *) malloc(sizeof(stmt_t));

	bool code_block = false;

	switch (token_list->tokens[*position].type) {
		case PRINT_TOKEN:
			*position += 1;
			statement->tag = STMT_PRINT;
			statement->data.stmt_print.expr = parse_expression(token_list, position);
			break;
		case VAR_TOKEN:
			*position += 1;
			statement->tag = STMT_DECLARATION;
			if (token_list->tokens[*position].type != IDENTIFIER_TOKEN) {
				parser_error(&token_list->tokens[*position], "expected identifier on variable declaration");
				break;
			}
			statement->data.stmt_declaration.identifier = &token_list->tokens[*position];
			*position += 1;

			if (token_list->tokens[*position].type == EQUAL_TOKEN) {
				*position += 1;
				statement->data.stmt_declaration.expr = parse_expression(token_list, position);
			} else {
				parser_error(&token_list->tokens[*position], "missing initialization on variable declaration");
			}
			break;
		case WHILE_TOKEN:
			*position += 1;
			statement->tag = STMT_WHILE;
			statement->data.stmt_while.condition = parse_expression(token_list, position);

			stmt_list_t *stmt_list = (stmt_list_t *) malloc(sizeof(stmt_list_t));
			init_stmt_list(stmt_list);

			// if (token_list->tokens[*position].type == LEFT_BRACE_TOKEN) {
				// *position += 1;
				// code_block = true;
				// while (token_list->tokens[*position].type != RIGHT_BRACE_TOKEN) {
					// if (token_list->tokens[*position].type == EOF_TOKEN) {
						// parser_error(&token_list->tokens[*position], "unexpected end of file, '{' not closed");
						// break;
					// }
					// stmt_t *stmt = parse_statement(token_list, position);
					// stmt_list_add(stmt_list, *stmt);
				// }
				// *position += 1;
			// } else {
				// stmt_t *stmt = parse_statement(token_list, position);
				// stmt_list_add(stmt_list, *stmt);
			// }
			while (token_list->tokens[*position].type != END_TOKEN) {
				if (token_list->tokens[*position].type == EOF_TOKEN) {
					parser_error(&token_list->tokens[*position], "unexpected end of file, code block not closed");
					break;
				}
				stmt_t *stmt = parse_statement(token_list, position);
				stmt_list_add(stmt_list, *stmt);
			}
			*position += 1;

			statement->data.stmt_while.stmt_list = stmt_list;
			break;
		default: 
			statement->tag = STMT_EXPRESSION;
			statement->data.stmt_expression.expr = parse_expression(token_list, position);

			break;
	}

	// if (!code_block token_list->tokens[*position].type == SEMICOLON_TOKEN) {
		// *position += 1;
	// } else {
		// parser_error(&token_list->tokens[*position], "; expected");
	// }

	return statement;
}

expr_t *parse_expression (token_list_t *token_list, int *position) {
	assert(*position < token_list->length);
	return parse_assignment(token_list, position);
}

expr_t *parse_assignment (token_list_t *token_list, int *position) {
	if (token_list->tokens[*position].type != IDENTIFIER_TOKEN ||
		token_list->tokens[*position + 1].type != EQUAL_TOKEN
		) {
		return parse_equality(token_list, position);
	}

	expr_t *assignment = (expr_t *) malloc(sizeof(expr_t));
	assignment->tag = EXPR_ASSIGNMENT;
	assignment->data.expr_assignment.identifier = token_list->tokens[*position].literal;
	*position += 1;
	assignment->data.expr_assignment.operator = &token_list->tokens[*position];
	*position += 1;
	assignment->data.expr_assignment.right = parse_expression(token_list, position);

	return assignment;
}

expr_t *parse_equality (token_list_t *token_list, int *position) {
	expr_t *left = (expr_t *) malloc(sizeof(expr_t));
	left = parse_comparison(token_list, position);

	expr_t *expr = (expr_t *) malloc(sizeof(expr_t));

	if (token_list->tokens[*position].type == EQUAL_EQUAL_TOKEN ||
		token_list->tokens[*position].type == BANG_EQUAL_TOKEN )
		{
		expr->data.expr_binary.left = left;
		expr->tag = EXPR_BINARY;
		expr->data.expr_binary.operator = &token_list->tokens[*position];
		*position += 1;
		if (*position >= token_list->length) {
			parser_error(&token_list->tokens[*position - 1], "unexpected end of file, expected expression");
			return expr;
		}
		expr->data.expr_binary.right = parse_equality(token_list, position);
	} else {
		expr = left;
	}

	return expr;
}

expr_t *parse_comparison (token_list_t *token_list, int *position) {
	expr_t *left = (expr_t *) malloc(sizeof(expr_t));
	left = parse_term(token_list, position);

	expr_t *expr = (expr_t *) malloc(sizeof(expr_t));

	if (token_list->tokens[*position].type == GREATER_TOKEN ||
		token_list->tokens[*position].type == LESS_TOKEN ||
		token_list->tokens[*position].type == GREATER_EQUAL_TOKEN ||
		token_list->tokens[*position].type == LESS_EQUAL_TOKEN ) 
		{
		expr->data.expr_binary.left = left;
		expr->tag = EXPR_BINARY;
		expr->data.expr_binary.operator = &token_list->tokens[*position];
		*position += 1;
		if (*position >= token_list->length) {
			parser_error(&token_list->tokens[*position - 1], "unexpected end of file, expected expression");
			return expr;
		}
		expr->data.expr_binary.right = parse_comparison(token_list, position);
	} else {
		expr = left;
	}

	return expr;
}

expr_t *parse_term (token_list_t *token_list, int *position) {
	expr_t *left = (expr_t *) malloc(sizeof(expr_t));
	left = parse_factor(token_list, position);

	expr_t *expr = (expr_t *) malloc(sizeof(expr_t));

	if (token_list->tokens[*position].type == PLUS_TOKEN ||
		token_list->tokens[*position].type == MINUS_TOKEN ) 
		{
		expr->data.expr_binary.left = left;
		expr->tag = EXPR_BINARY;
		expr->data.expr_binary.operator = &token_list->tokens[*position];
		*position += 1;
		if (*position >= token_list->length) {
			parser_error(&token_list->tokens[*position - 1], "unexpected end of file, expected expression");
			return expr;
		}
		expr->data.expr_binary.right = parse_term(token_list, position);
	} else {
		expr = left;
	}

	return expr;
}

expr_t *parse_factor (token_list_t *token_list, int *position) {
	expr_t *left = (expr_t *) malloc(sizeof(expr_t));
	left = parse_unary(token_list, position);

	expr_t *expr = (expr_t *) malloc(sizeof(expr_t));

	if (token_list->tokens[*position].type == STAR_TOKEN ||
		token_list->tokens[*position].type == SLASH_TOKEN ) 
		{
		expr->data.expr_binary.left = left;
		expr->tag = EXPR_BINARY;
		expr->data.expr_binary.operator = &token_list->tokens[*position];
		*position += 1;
		if (*position >= token_list->length) {
			parser_error(&token_list->tokens[*position - 1], "unexpected end of file, expected expression");
			return expr;
		}
		expr->data.expr_binary.right = parse_factor(token_list, position);
	} else {
		expr = left;
	}

	return expr;
}

expr_t *parse_unary (token_list_t *token_list, int *position) {
	assert(*position < token_list->length);
	if (token_list->tokens[*position].type != BANG_TOKEN &&
		token_list->tokens[*position].type != MINUS_TOKEN &&
		token_list->tokens[*position].type != HASH_TOKEN
		) {
		return parse_primary(token_list, position);
	} 

	expr_t *unary = (expr_t *) malloc(sizeof(expr_t));
	unary->tag = EXPR_UNARY;
	unary->data.expr_unary.operator = &token_list->tokens[*position];

	*position += 1;
	if (*position >= token_list->length) {
		parser_error(&token_list->tokens[*position - 1], "unexpected end of file, expected expression");
		return unary;
	}

	unary->data.expr_unary.right = parse_unary(token_list, position);
	return unary;
}

expr_t *parse_primary(token_list_t *token_list, int *position) {
	assert(*position < token_list->length);
	expr_t *primary = (expr_t *) malloc(sizeof(expr_t));

	switch (token_list->tokens[*position].type) {
		case STRING_LITERAL_TOKEN:
			primary->tag = EXPR_STRING_LITERAL;
			primary->data.expr_string_literal.value = (char *) token_list->tokens[*position].literal;
			break;
		case INTEGER_LITERAL_TOKEN:
			primary->tag = EXPR_INTEGER_LITERAL;
			primary->data.expr_integer_literal.value = (int *) token_list->tokens[*position].literal;
			break;
		case LEFT_PAREN_TOKEN:
			*position += 1;
			primary = parse_expression(token_list, position);

			if (token_list->tokens[*position].type != RIGHT_PAREN_TOKEN) {
				parser_error(&token_list->tokens[*position], "expected ')'");
				// break;
			}
			break;
		case TRUE_TOKEN:
			primary->tag = EXPR_BOOLEAN_LITERAL;
			primary->data.expr_boolean_literal.value = malloc(sizeof(bool));
			*primary->data.expr_boolean_literal.value = true;
			break;
		case FALSE_TOKEN:
			primary->tag = EXPR_BOOLEAN_LITERAL;
			primary->data.expr_boolean_literal.value = malloc(sizeof(bool));
			*primary->data.expr_boolean_literal.value = false;
			break;
		case IDENTIFIER_TOKEN:
			primary->tag = EXPR_IDENTIFIER;
			primary->data.expr_identifier.value = token_list->tokens[*position].literal;
			break;
		default: 
			parser_error(&token_list->tokens[*position], "unexpected token");
			primary->tag = EXPR_UNDEFINED;
			break;
	}

	*position += 1;

	return primary;
}

