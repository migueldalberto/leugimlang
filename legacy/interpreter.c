#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "environment.h"

#include "interpreter.h"

const char *lang_type_to_string(enum LANG_TYPE type) {
	switch (type) {
		case LANG_STRING: return "STRING";
		case LANG_INTEGER: return "INTEGER";
		case LANG_BOOLEAN: return "BOOLEAN";
		case LANG_NULL: return "NULL";
		default: return "_";
	}
}

void interpret(stmt_list_t *stmt_list, env_t *upper_env) {
	env_t env;
	init_env(&env);
	env_add_upper(&env, upper_env);

	for (int i = 0; i < stmt_list->length; ++i)
	{
		interpret_stmt(&stmt_list->stmts[i], &env);
		// for (int j = 0; j < env.length; ++j) {
			// printf("ENV: %d\t%s\t%s\n", j, env.keys[j], env.values[j]);
		// }
		
	}
}

void interpret_stmt(stmt_t *stmt, env_t *env) {
	eval_t eval;
	switch(stmt->tag) {
		case STMT_PRINT:
			eval = evaluate(stmt->data.stmt_print.expr, env);
			switch (eval.type) {
				case LANG_STRING:
					printf("%s\n", eval.data);
					break;
				case LANG_INTEGER:
					printf("%d\n", *((int *) eval.data));
					break;
				case LANG_BOOLEAN:
					if(*((bool *) eval.data))
						printf("true\n");
					else 
						printf("false\n");
					break;
			}
			break;
		case STMT_EXPRESSION:
			eval = evaluate(stmt->data.stmt_expression.expr, env);
			break;
		case STMT_WHILE:
			eval = evaluate(stmt->data.stmt_while.condition, env);
			
			while (*((bool *) eval.data)) {
				for (int i = 0; i < stmt->data.stmt_while.stmt_list->length; ++i)
					interpret_stmt(&stmt->data.stmt_while.stmt_list->stmts[i], env);
				eval = evaluate(stmt->data.stmt_while.condition, env);
				printf("condition_eval: type = %s\n\tdata = %d\n",
					lang_type_to_string(eval.type),
					*((bool *) eval.data)
				);
			}
			break;
		case STMT_DECLARATION:
			eval = evaluate(stmt->data.stmt_declaration.expr, env);
			env_add(env, stmt->data.stmt_declaration.identifier->literal, eval.data, eval.type);
			break;
	}
}

eval_t evaluate(expr_t *expr, env_t *env) {
	eval_t eval;
	switch(expr->tag) {
		case EXPR_UNARY: return evaluate_unary(expr, env);
		case EXPR_BINARY: return evaluate_binary(expr, env);
		case EXPR_INTEGER_LITERAL: 
			eval.type = LANG_INTEGER;
			eval.data = expr->data.expr_integer_literal.value;
			return eval;
		case EXPR_STRING_LITERAL:
			eval.type = LANG_STRING;
			eval.data = expr->data.expr_string_literal.value;
			return eval;
		case EXPR_BOOLEAN_LITERAL:
			eval.type = LANG_BOOLEAN;
			eval.data = expr->data.expr_boolean_literal.value;
			return eval;
		case EXPR_IDENTIFIER:
			env_entry ee = env_get(env, expr->data.expr_identifier.value);
			eval.type = ee.type;
			eval.data = ee.value;
			return eval;
		case EXPR_ASSIGNMENT:
			eval = evaluate(expr->data.expr_assignment.right, env);
			env_set(env, expr->data.expr_assignment.identifier, eval.data);
			return eval;
	}
}

eval_t evaluate_unary(expr_t *expr, env_t *env) {
	assert(expr->tag == EXPR_UNARY);

	eval_t right = evaluate(expr->data.expr_unary.right, env);
	eval_t eval;

	switch (expr->data.expr_unary.operator->type) {
		case MINUS_TOKEN:
			if (right.type != LANG_INTEGER) {
				fprintf(stderr, "type error: expected INTEGER, got %s\n", lang_type_to_string(eval.type));
				eval.type = LANG_NULL;
			} else {
				int *new = (int *) malloc(sizeof(int));
				*new = - *((int *) right.data);
				eval.data = new;
			}
			break;
		case BANG_TOKEN:
			if (right.type != LANG_BOOLEAN) {
				fprintf(stderr, "type error: expected BOOLEAN, got %s\n", lang_type_to_string(eval.type));
				eval.type = LANG_NULL;
			} else {
				bool *new = (bool *) malloc(sizeof(int));
				*new = ! *((bool *) right.data);
				free(eval.data);
				eval.data = new;
			}
			break;
		case HASH_TOKEN:
			if (right.type != LANG_STRING) {
				fprintf(stderr, "type error: expected STRING, got %s\n", lang_type_to_string(eval.type));
				eval.type = LANG_NULL;
			} else {
				eval.type = LANG_INTEGER;
				eval.data = malloc(sizeof(int));
				*((int *) eval.data) = strlen(right.data);
			}
			break;
	}

	free(right.data);
	return eval;
}

eval_t evaluate_binary(expr_t *expr, env_t *env) {
	assert(expr->tag == EXPR_BINARY);

	expr_t *left = expr->data.expr_binary.left;
	expr_t *right = expr->data.expr_binary.right;

	eval_t left_eval = evaluate(left, env);
	eval_t right_eval = evaluate(right, env);

	eval_t eval;
	eval.type = LANG_NULL;
	eval.data = malloc(sizeof(char));

	if (left_eval.type == LANG_INTEGER &&
		right_eval.type == LANG_INTEGER) {
		eval.type = LANG_INTEGER;

		switch (expr->data.expr_binary.operator->type) {
			case PLUS_TOKEN:
				eval.data = malloc(sizeof(int));
				*((int *) eval.data) = *((int *) left_eval.data) +
					*((int *) right_eval.data);
				break;
			case MINUS_TOKEN:
				eval.data = malloc(sizeof(int));
				*((int *) eval.data) = *((int *) left_eval.data) -
					*((int *) right_eval.data);
				break;
			case STAR_TOKEN:
				eval.data = malloc(sizeof(int));
				*((int *) eval.data) = *((int *) left_eval.data) *
					*((int *) right_eval.data);
				break;
			case SLASH_TOKEN:
				eval.data = malloc(sizeof(int));
				*((int *) eval.data) = *((int *) left_eval.data) /
					*((int *) right_eval.data);
				break;
			case GREATER_TOKEN:
				eval.data = malloc(sizeof(bool));
				eval.type = LANG_BOOLEAN;
				*((bool *) eval.data) = *((int *) left_eval.data) >
					*((int *) right_eval.data);
				break;
			case LESS_TOKEN:
				eval.data = malloc(sizeof(bool));
				eval.type = LANG_BOOLEAN;
				*((bool *) eval.data) = *((int *) left_eval.data) <
					*((int *) right_eval.data);
				break;
			case GREATER_EQUAL_TOKEN:
				eval.data = malloc(sizeof(bool));
				eval.type = LANG_BOOLEAN;
				*((bool *) eval.data) = *((int *) left_eval.data) >=
					*((int *) right_eval.data);
				break;
			case LESS_EQUAL_TOKEN:
				eval.data = malloc(sizeof(bool));
				eval.type = LANG_BOOLEAN;
				*((bool *) eval.data) = *((int *) left_eval.data) <=
					*((int *) right_eval.data);
				break;
			case EQUAL_EQUAL_TOKEN:
				eval.data = malloc(sizeof(bool));
				eval.type = LANG_BOOLEAN;
				*((bool *) eval.data) = *((int *) left_eval.data) ==
					*((int *) right_eval.data);
				break;
			case BANG_EQUAL_TOKEN:
				eval.data = malloc(sizeof(bool));
				eval.type = LANG_BOOLEAN;
				*((bool *) eval.data) = *((int *) left_eval.data) !=
					*((int *) right_eval.data);
				break;
		}
	} else if (left_eval.type == LANG_STRING &&
		right_eval.type == LANG_STRING
	) {
		switch (expr->data.expr_binary.operator->type) {
			case PLUS_TOKEN:
				eval.type = LANG_STRING;
				eval.data = malloc((strlen(left_eval.data) + strlen(right_eval.data) + 1) * sizeof(char));
				((char *) eval.data)[0] = '\0';
				strcpy(eval.data, left_eval.data);
				strcat(eval.data, right_eval.data);
				break;
			case EQUAL_EQUAL_TOKEN:
				eval.type = LANG_BOOLEAN;
				eval.data = malloc(sizeof(bool));
				*((bool *) eval.data) = !strcmp(left_eval.data, right_eval.data);
				break;
			default:
				fprintf(
					stderr, 
					"invalid operator '%s' for types STRING and STRING\n",
					expr->data.expr_binary.operator->lexeme
				);
				break;
		}
	} else {
		fprintf(stderr, "evaluation not implemented for %s %s %s\n",
			lang_type_to_string(left_eval.type),
			expr->data.expr_binary.operator->lexeme,
			lang_type_to_string(right_eval.type)
		);
	}

	free(left_eval.data);
	free(right_eval.data);

	return eval;
}
