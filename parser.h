#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>

#include "stmt.h"
#include "expr.h"
#include "parser.h"
#include "scanner.h"

stmt_list_t *parse_tokens(token_list_t *token_list);
stmt_t *parse_statement (token_list_t *token_list, int *position);
expr_t *parse_assignment (token_list_t *token_list, int *position);
expr_t *parse_expression (token_list_t *token_list, int *position);
expr_t *parse_equality (token_list_t *token_list, int *position);
expr_t *parse_comparison (token_list_t *token_list, int *position);
expr_t *parse_term (token_list_t *token_list, int *position);
expr_t *parse_factor (token_list_t *token_list, int *position);
expr_t *parse_unary (token_list_t *token_list, int *position);
expr_t *parse_primary (token_list_t *token_list, int *position);

#endif
