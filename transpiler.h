#ifndef TRANSPILER_H
#define TRANSPILER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "stmt.h"
#include "expr.h"

void js_transpile(FILE *out, stmt_list_t *stmt_list);
char *js_transpile_stmt(stmt_t *stmt);
char *stringify_expression(expr_t expr);

#endif
