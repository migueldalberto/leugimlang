#ifndef TOKEN_H
#define TOKEN_H

enum token_type {
	LEFT_PAREN_TOKEN, RIGHT_PAREN_TOKEN,
	LEFT_BRACE_TOKEN, RIGHT_BRACE_TOKEN,
	LEFT_BRACKET_TOKEN, RIGHT_BRACKET_TOKEN,

	DOT_TOKEN, COMMA_TOKEN, SEMICOLON_TOKEN, COLON_TOKEN,
	
	PLUS_TOKEN, MINUS_TOKEN, STAR_TOKEN, SLASH_TOKEN, HASH_TOKEN,

	GREATER_TOKEN, GREATER_EQUAL_TOKEN,
	LESS_TOKEN, LESS_EQUAL_TOKEN,
	BANG_TOKEN, BANG_EQUAL_TOKEN,
	EQUAL_TOKEN, EQUAL_EQUAL_TOKEN,

	INTEGER_LITERAL_TOKEN, STRING_LITERAL_TOKEN,

	PRINT_TOKEN,
	VAR_TOKEN,
	WHILE_TOKEN,
	TRUE_TOKEN,
	FALSE_TOKEN,
	END_TOKEN,
	IF_TOKEN,
	ELSE_TOKEN,
	IDENTIFIER_TOKEN,

	EOF_TOKEN
};

struct keyword {
	char *string;
	enum token_type type;
};

typedef struct {
	int position;
	char *lexeme;
	void *literal;
	enum token_type type;
} token_t;

typedef struct {
	int capacity;
	int length;
	token_t *tokens;
} token_list_t;

void init_token_list(token_list_t *list);
void token_list_add(token_list_t *list, token_t token);

const char *token_type_to_string(enum token_type type);
void print_token(token_t token);
void init_token(token_t *token, int position, char *lexeme, void *literal, enum token_type type);

#endif
