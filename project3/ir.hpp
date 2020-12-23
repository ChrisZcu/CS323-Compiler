#include <vector>
#include <string>

#ifndef AST
#define AST
#include "ast.hpp"
#endif

using namespace std;

typedef struct code_node
{
    vector<string> code_list;
} code_node;
void ir_translate(struct ast *ast);
void new_label(string &name);
void new_temp(string &name);
code_node translate_exp(struct ast *exp, string place);
code_node translate_stmt(struct ast *stmt);
code_node translate_cond(struct ast *exp, string label_true, string label_false);
void translate_args(struct ast *args, vector<string> &arg_list);
void function_handle(struct ast *fun_dec);
void function_init(struct ast *fun_dec);
void function_def_list(struct ast *def_list);
void function_stmt_list(struct ast *stmt_list);
pair<bool, string> get_exp_number(struct ast *exp);
void exp_opt(struct ast *exp, string &t);
void array_handle(struct ast *exp, string &final_addr);
int get_struct_size(val_d *_struct);
int get_field_size(val_d val);
void struct_handle(struct ast *exp, string &final_addr);
