#include "scanner.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define N_OF_KEYWORDS 8

struct keyword keywords[] = {
	{ .string = "escreveR", 	.type = PRINT_TOKEN },
	{ .string = "var", 			.type = VAR_TOKEN },
	{ .string = "verdadeiro", 	.type = TRUE_TOKEN },
	{ .string = "falso", 		.type = FALSE_TOKEN },
	{ .string = "enquanto", 	.type = WHILE_TOKEN },
	{ .string = "se", 			.type = IF_TOKEN },
	{ .string = "senao", 		.type = ELSE_TOKEN },
	{ .string = "fim", 			.type = END_TOKEN }
};

token_list_t *get_tokens(char *source) {
	token_list_t *tokens = (token_list_t *) malloc(sizeof(token_list_t));
	init_token_list(tokens);

	int start = 0, 
		position = 0;
	
	char *get_lexeme() {
		char *lexeme = (char *) malloc((position - start + 2) * sizeof(char));
		lexeme[position - start + 1] = '\0';
		strncpy(lexeme, &source[start], position - start + 1);
		return lexeme;
	}

	void add_token(enum token_type type, void *literal) {
		token_t token = { 
			.position = start,
			.type = type,
			.lexeme = get_lexeme(),
			.literal = literal
		};

		token_list_add(tokens, token);
	}

	void scanner_error (const char *msg) {
		fprintf(stderr, "[SCANNER] [at %d] - %s\n", position, msg);
	}

	bool is_digit(char c) {
		return c <= '9' && c >= '0';
	}
	
	bool is_letter(char c) {
		return (c >= 'a' && c <= 'z') ||
			(c >= 'A' && c <= 'Z');
	}

	bool expect(char c) {
		if (source[position + 1] == c) {
			++position;
			return true;
		} else
			return false;
	}

	while (source[position] != '\0') {
		start = position;

		switch(source[position]) {
			case '.': add_token(DOT_TOKEN, NULL); break;
			case ',': add_token(COMMA_TOKEN, NULL); break;
			case ';': add_token(SEMICOLON_TOKEN, NULL); break;
			case ':': add_token(COLON_TOKEN, NULL); break;
			case '+': add_token(PLUS_TOKEN, NULL); break;
			case '-': add_token(MINUS_TOKEN, NULL); break;
			case '*': add_token(STAR_TOKEN, NULL); break;
			case '#': add_token(HASH_TOKEN, NULL); break;
			case '/': 
				if (source[position + 1] == '/') 
					while (source[position + 1] != '\n' && source[position + 1] != '\r' && source[position + 1] != '\0')
						++position;
				else 
					add_token(SLASH_TOKEN, NULL); 
				break;
			case '=':
				if(expect('=')) add_token(EQUAL_EQUAL_TOKEN, NULL);
				else 			add_token(EQUAL_TOKEN, NULL);
				break;
			case '!':
				if(expect('=')) add_token(BANG_EQUAL_TOKEN, NULL);
				else 			add_token(BANG_TOKEN, NULL);
				break;
			case '<':
				if(expect('=')) add_token(LESS_EQUAL_TOKEN, NULL);
				else 			add_token(LESS_TOKEN, NULL);
				break;
			case '>':
				if(expect('=')) add_token(GREATER_EQUAL_TOKEN, NULL);
				else 			add_token(GREATER_TOKEN, NULL);
				break;
			case '(': add_token(LEFT_PAREN_TOKEN, NULL); break;
			case ')': add_token(RIGHT_PAREN_TOKEN, NULL); break;
			case '{': add_token(LEFT_BRACE_TOKEN, NULL); break;
			case '}': add_token(RIGHT_BRACE_TOKEN, NULL); break;
			case '[': add_token(LEFT_BRACKET_TOKEN, NULL); break;
			case ']': add_token(RIGHT_BRACKET_TOKEN, NULL); break;
			case '"':
				++position;
				while (source[position] != '"') {
					if (source[position] == '\0') {
						scanner_error("unexpected end of file, string unended");
						break;
					}

					++position;
				}

				char *literal = (char *) malloc(position - start * sizeof(char));
				literal[position - start - 1] = '\0';
				strncpy(literal, &source[start + 1], position - start - 1);

				add_token(STRING_LITERAL_TOKEN, literal);
				break;
			case ' ': 
			case '\r': 
			case '\n': 
			case '\t': 
			break;
			default: 
				if (is_digit(source[position])) {
					bool reached_decimal_separator = false;
					while (is_digit(source[position + 1]))
						++position;

					int *literal = (int *) malloc(sizeof(int));
					*literal = atoi(get_lexeme());
					add_token(INTEGER_LITERAL_TOKEN, (void *) literal);
				} else {
					while (	is_letter(source[position + 1]) ||
							source[position + 1] == '_' ||
							is_digit(source[position + 1]) 
						)
						++position;

					char *word = get_lexeme();
					int i = 0;
					bool is_keyword = false;

					while (i < N_OF_KEYWORDS) {
						if(!strcmp(word, keywords[i].string)) {
							is_keyword = true;
							add_token(keywords[i].type, NULL);
						}
						++i;
					}

					if (!is_keyword) {
						add_token(IDENTIFIER_TOKEN, word);
					}
				}
			break;
		}

		++position;
	}

	token_t eof_token = {
		.position = position,
		.type = EOF_TOKEN,
		.lexeme = "\0"
	};

	token_list_add(tokens, eof_token);

	return tokens;
}

