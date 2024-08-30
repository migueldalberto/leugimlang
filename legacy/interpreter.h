#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "stmt.h"
#include "expr.h"
#include "environment.h"
#include "lang_type.h"

typedef struct eval {
	enum LANG_TYPE type;
	void *data;
} eval_t;

void interpret(stmt_list_t *stmt_list, env_t *upper_env);
void interpret_stmt(stmt_t *stmt, env_t *env);
eval_t evaluate(expr_t *ast, env_t *env);
eval_t evaluate_unary(expr_t *ast, env_t *env);
eval_t evaluate_binary(expr_t *ast, env_t *env);

#endif
