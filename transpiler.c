#include "transpiler.h"

void js_transpile(FILE *out, stmt_list_t *stmt_list) {
	fputs("<!DOCTYPE html>\n", out);
	fputs("<html>\n", out);
	fputs("<head>\n", out);
	fputs("<style>\n", out);

	FILE *css_stdstyle = fopen("leugimstyle.css", "r");
	char c;
	assert(css_stdstyle != NULL);
	while ((c = fgetc(css_stdstyle )) != EOF) {
		fputc(c, out);
	}
	fclose(css_stdstyle);

	fputs("</style>\n", out);

	fputs("</head>\n", out);
	fputs("<body>\n", out);
	fputs("<script>\n", out);

	FILE *js_stdlib = fopen("leugimlib.js", "r");
	assert(js_stdlib != NULL);

	while ((c = fgetc(js_stdlib)) != EOF) {
		fputc(c, out);
	}

	fclose(js_stdlib);

	fputs("</script>\n", out);
	fputs("<script>\n", out);

	for (int i = 0; i < stmt_list->length; ++i) {
		char *line = js_transpile_stmt(&stmt_list->stmts[i]);
		fputs(line, out);
		free(line);
	}

	fputs("</script>\n", out);

	fputs("</body>\n", out);
	fputs("</html>\n", out);
}

char *js_transpile_stmt(stmt_t *stmt) {
	int buf_size = 120;
	char *line = (char *) malloc(buf_size * sizeof(char));
	line[0] = '\0';

	switch(stmt->tag) {
		case STMT_PRINT:
			strcat(line, "print(");
			strcat(line, stringify_expression(*stmt->data.stmt_print.expr));
			strcat(line, ")");
			break;
		case STMT_EXPRESSION:
			strcat(line, stringify_expression(*stmt->data.stmt_expression.expr));
			break;
		case STMT_WHILE:
			strcat(line, "while (");
			strcat(line, stringify_expression(*stmt->data.stmt_while.condition));
			strcat(line, ") {\n");
			for (int i = 0; i < stmt->data.stmt_while.stmt_list->length; ++i) {
				char *while_line = js_transpile_stmt(&stmt->data.stmt_while.stmt_list->stmts[i]);
				strcat(line, while_line);
				free(while_line);
			}
			strcat(line, "}\n");
			break;
		case STMT_DECLARATION:
			strcat(line, "let ");
			strcat(line, stmt->data.stmt_declaration.identifier->literal);
			strcat(line, " = ");
			strcat(line, stringify_expression(*stmt->data.stmt_declaration.expr));
			break;
	}

	strcat(line, "\n");
	return line;
}

char *stringify_expression(expr_t expr) {
	char *str;
	switch (expr.tag) {
		case EXPR_UNDEFINED:
			str = "undefined";
			break;
		case EXPR_IDENTIFIER:
			str = expr.data.expr_identifier.value;
			break;
		case EXPR_INTEGER_LITERAL:
			str = (char *) malloc(40 * sizeof(char));
			sprintf(str, "%d", *(int *) expr.data.expr_integer_literal.value);
			break;
		case EXPR_STRING_LITERAL:
			str = expr.data.expr_string_literal.value;
			break;
		case EXPR_UNARY:
			str = (char *) malloc(40 * sizeof(char));
			str[0] = '\0';
			strcat(str, (char *) expr.data.expr_unary.operator->literal);
			strcat(str, " ");
			strcat(str, stringify_expression(*expr.data.expr_unary.right));
			break;
		case EXPR_ASSIGNMENT:
			str = (char *) malloc(40 * sizeof(char));
			str[0] = '\0';
			strcat(str, expr.data.expr_assignment.identifier);
			strcat(str, " ");
			strcat(str, (char *) expr.data.expr_assignment.operator->lexeme);
			strcat(str, " ");
			strcat(str, stringify_expression(*expr.data.expr_assignment.right));
			break;
		case EXPR_BINARY:
			str = (char *) malloc(40 * sizeof(char));
			str[0] = '\0';
			strcat(str, stringify_expression(*expr.data.expr_binary.left));
			strcat(str, " ");
			strcat(str, (char *) expr.data.expr_binary.operator->lexeme);
			strcat(str, " ");
			strcat(str, stringify_expression(*expr.data.expr_binary.right));
			break;
	}

	return str;
}
