#include <vector>
#include <string>

#ifndef AST
#define AST
#include "ast.hpp"
#endif

using namespace std;
/* args_count=2:
 * 	GOTO x
 * 	RETURN x
 * 	ARG x
 * 	PARAM x
 * 	READ x
 * 	WRITE x
 * args_count=3:
 * 	LABEL x :
 * 	FUNCTION f :
 * 	x := y
 * 	x := &y
 * 	x := *y
 * 	*x := y
 * 	DEC x [size]
 * args_count=4:
 * 	x := CALL f
 * args_count=5:
 * 	x := y + z
 * 	x := y - z
 * 	x := y * z
 * 	x := y / z
 * args_count=6:
 * IF x [relop] y GOTO z
 */

typedef struct code_node
{
    vector<string>code_list;
} code_node;
void ir_translate(struct ast *ast);
void new_label(string &name);
void new_temp(string &name);
void add_code(int args_count, ...);
code_node translate_exp(struct ast *exp, string place);
code_node translate_stmt(struct ast *stmt);
code_node translate_cond(struct ast*exp, string label_true, string label_false);
code_node translate_args(struct ast*args);
void function_handle(struct ast* fun_dec);
void function_init(struct ast *fun_dec);
void function_def_list(struct ast *def_list);
void function_stmt_list(struct ast *stmt_list);