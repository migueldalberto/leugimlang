#include "token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_token_list(token_list_t *list) {
	list->capacity = 1;
	list->length = 0;
	list->tokens = (token_t *) malloc(sizeof(token_t));
}

void token_list_add(token_list_t *list, token_t token) {
	if (list->length + 1 >= list->capacity) {
		list->capacity *= 2;
		token_t *new_list = (token_t *) malloc(list->capacity * sizeof(token_t));

		for (int i = 0 ; i < list->length; ++i)
			new_list[i] = list->tokens[i];

		free(list->tokens);
		list->tokens = new_list;
	}

	list->tokens[list->length] = token;
	list->length += 1;
}

const char *token_type_to_string(enum token_type type) {
	switch(type) {
		case LEFT_PAREN_TOKEN: return "LEFT_PAREN";
		case RIGHT_PAREN_TOKEN: return "RIGHT_PAREN";
		case LEFT_BRACE_TOKEN: return "LEFT_BRACE";
		case RIGHT_BRACE_TOKEN: return "RIGHT_BRACE";
		case LEFT_BRACKET_TOKEN: return "LEFT_BRACKET";
		case RIGHT_BRACKET_TOKEN: return "RIGHT_BRACKET";
		case DOT_TOKEN: return "DOT";
		case COMMA_TOKEN: return "COMMA";
		case SEMICOLON_TOKEN: return "SEMICOLON";
		case COLON_TOKEN: return "COLON";
		case PLUS_TOKEN: return "PLUS";
		case MINUS_TOKEN: return "MINUS";
		case STAR_TOKEN: return "STAR";
		case SLASH_TOKEN: return "SLASH";
		case HASH_TOKEN: return "HASH";
		case BANG_TOKEN: return "BANG";
		case BANG_EQUAL_TOKEN: return "BANG_EQUAL";
		case EQUAL_TOKEN: return "EQUAL";
		case EQUAL_EQUAL_TOKEN: return "EQUAL_EQUAL";
		case INTEGER_LITERAL_TOKEN: return "INTEGER_LITERAL";
		case STRING_LITERAL_TOKEN: return "STRING_LITERAL";
		case PRINT_TOKEN: return "PRINT";
		case TRUE_TOKEN: return "TRUE";
		case FALSE_TOKEN: return "FALSE";
		case VAR_TOKEN: return "VAR";
		case WHILE_TOKEN: return "WHILE";
		case IF_TOKEN: return "IF";
		case ELSE_TOKEN: return "ELSE";
		case IDENTIFIER_TOKEN: return "IDENTIFIER";
		case EOF_TOKEN: return "EOF";
		default: return "_";
	}
}

void print_token(token_t token) {
	if (token.type == INTEGER_LITERAL_TOKEN) {
		printf(
			"[%s] at %d, '%s' = %d\n", 
			token_type_to_string(token.type), 
			token.position,
			token.lexeme,
			*((int *) token.literal)
			);
	} else {
		printf(
			"[%s] at %d, '%s'\n", 
			token_type_to_string(token.type), 
			token.position,
			token.lexeme
			);
	}
}

void init_token(token_t *token, int position, char *lexeme, void *literal, enum token_type type) {
	token->position = position;
	token->lexeme = lexeme;
	token->literal = literal;
	token->type = type;
}
