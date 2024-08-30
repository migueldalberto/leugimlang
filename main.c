#include <stdio.h>
#include <stdlib.h>

#include "scanner.h"
#include "parser.h"
#include "stmt.h"
#include "token.h"
#include "transpiler.h"

void run(char *source);

int main(int argc, char *argv[]) {
	if (argc == 1) {
		fprintf(stderr, "uso: %s [arquivo]\n", argv[0]);
		return 0;
	}

	FILE *f = fopen(argv[1], "r");
	if (f == NULL) {
		fprintf(stderr, "erro, não foi possível abrir o arquivo '%s'\n", argv[1]);
	}

	int capacity = 16;
	int length = 0;
	char *buf = (char *) malloc(capacity * sizeof(char));
	char c;

	while ((c = fgetc(f)) != EOF) {
		buf[length] = c;
		++length;

		if (length == capacity - 1) {
			capacity *= 2;
			char *new_buf = (char *) malloc(capacity * sizeof(char));

			for (int i = 0; i < length; ++i)
				new_buf[i] = buf[i];

			free(buf);
			buf = new_buf;
		}
	}

	fclose(f);

	buf[length] = '\0';
	run(buf);

	free(buf);

	return 0;
}

void run(char *source) {
	token_list_t *token_list = get_tokens(source);

	// for (int i = 0; i < token_list->length; ++i) {
		// print_token(token_list->tokens[i]);
	// }

	stmt_list_t *stmt_list = parse_tokens(token_list);

	// for (int i = 0; i< stmt_list->length; ++i)
		// stmt_simple_print(&stmt_list->stmts[i]);
	// interpret(stmt_list, env);
	
	js_transpile(stdout, stmt_list);
}

