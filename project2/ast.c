#ifndef CTYPE_H
#define CTYPE_H
#include <ctype.h>
#endif

#ifndef STDARG_H
#define STDARG_H
#include <stdarg.h>
#endif

#ifndef LEX_YY_C
#define LEX_YY_C
#include "lex.yy.c"
#endif

#ifndef LL_VARIABLE
#define LL_VARIABLE
#include "symtab_ll_variable.c"
#endif

#ifndef LL_FUNCTION
#define LL_FUNCTION
#include "symtab_ll_function.c"
#endif

struct variable_list *variable_head = NULL;
struct symtab_function *function_head = NULL;

struct ast
{
    char *name;
    char *value;
    int lineno;

    struct ast *next_layer;
    struct ast *next_neighbor;
};

struct variable_list
{
    symtab_variable *symtab_variable;
    struct variable_list *childe;
    struct variable_list *parent;
};

struct ast *new_ast(char *name, int num, ...)
{
    va_list valist;
    struct ast *node = (struct ast *)malloc(sizeof(struct ast));
    struct ast *tmp = (struct ast *)malloc(sizeof(struct ast));
    node->name = name;

    va_start(valist, num);

    if (num > 0)
    {
        tmp = va_arg(valist, struct ast *);
        node->next_layer = tmp;
        node->lineno = tmp->lineno;
        num--;
        while (num > 0)
        {
            tmp->next_neighbor = va_arg(valist, struct ast *);
            tmp = tmp->next_neighbor;
            num--;
        }
    }

    return node;
}

struct ast *new_node(char *name, char *value, int lineno)
{
    struct ast *node = (struct ast *)malloc(sizeof(struct ast));
    node->name = name;
    node->value = value;
    node->lineno = lineno;

    return node;
}
char *get_type(struct ast *ast);
int check_exp_type(struct ast *ast);
void scan_tree(struct ast *ast, int level);
void variable_insert();
void global_charge(struct ast *ast);
void func_args_charge(struct ast *ast);
void local_charge(struct ast *ast);
void dec_list_charge(char *type, struct ast *ast);
void dec_charge(char *type, struct ast *ast);
int var_dec_charge(char *type, struct ast *ast);
int func_args_compare(struct ast *args1, struct ast *args2);
void func_return_type_compare(struct ast *ast);
int variable_check_define(char *name);
int variable_check_define_global(char *name);
void variable_delete(int is_struct);
void print_variable_head();
VAL_VARIABLE get_variable_value(char *name);
VAL_VARIABLE get_struct_field_value(struct symtab_variable *struct_field, char *name);
int array_check(struct ast *exp);
int continue_parse = 1;
int array_check(struct ast *exp)
{
    int dim = 0;
    struct ast *exp_next = exp->next_layer;
    if (exp_next->next_neighbor == NULL)
    { //variable

        char *name = exp_next->value;

        struct variable_list *tmp_head = variable_head;

        while (tmp_head->parent != NULL)
        {
            VAL_VARIABLE a = variable_symtab_lookup(tmp_head->symtab_variable, name);
            if (strcmp(a.type, "null_false"))
            {
                return a.dim;
            }
            tmp_head = tmp_head->parent;
        }
        printf("Error type 1 at Line%d: variable %s is not defined before.\n", exp->lineno, name);
        continue_parse = 0;
        return -1;
    }
    else if (!strcmp(exp_next->name, "ID"))
    { //function
        //FIXME function return type
        VAL_FUNTION fun = function_symtab_lookup(function_head, exp_next->value);
    }
    else
    { //struct
        char *struct_type = get_type(exp_next);
        char *field_name = exp_next->next_neighbor->next_neighbor->value;
        if (!strcmp(struct_type, "int") ||
            !strcmp(struct_type, "float") ||
            !strcmp(struct_type, "char") ||
            !strcmp(struct_type, "boolean"))
        {
            printf("Error type 13 at Line %d: non-struct object access dot\n", exp_next->lineno);
        }
        else
        {

            VAL_VARIABLE struct_field = get_variable_value(struct_type);
            VAL_VARIABLE value = get_struct_field_value(struct_field.field, field_name);
            if (!strcmp(value.type, "null_false"))
            {
                printf("Error type 14 at Line %d: no such member: %s\n", exp_next->lineno, field_name);
            }

            return value.dim;
        }
    }
    return -1;
}
void print_variable_head()
{
    struct variable_list *tmp_head = variable_head;
    int undefine_ = 1;
    while (tmp_head->parent != NULL)
    {
        symtab_variable *ptr = tmp_head->symtab_variable;
        while (ptr->next != NULL)
        {
            printf("%s, ", ptr->entry.key);
            ptr = ptr->next;
        }
        tmp_head = tmp_head->parent;
        printf("\n");
    }
}
//TODO check the semantic

void parsetree(struct ast *ast, int level)
{
    // printf("%s, %d\n", ast->name, ast->lineno);
    if (!continue_parse)
    {
        return;
    }

    int scope = 0;
    if (ast != NULL)
    {
        if (ast->name != NULL)
        {
            if (ast->lineno != -1)
            {
                //check assign, function need return type, lack of specifier type.
                //check operation of unsuitable operation

                // if (!strcmp(ast->name, "CompSt"))
                // {
                //     scope = 1;
                //     // add new scope
                //     variable_insert();
                // }
                //global variable
                if (!strcmp(ast->name, "ExtDef"))
                {

                    if (!strcmp(ast->next_layer->next_neighbor->name, "SEMI")) //struct

                    { //global, do not delete

                        variable_insert();
                    }
                    else if (!strcmp(ast->next_layer->next_neighbor->name, "FunDec"))
                    {
                        scope = 1;
                        variable_insert();
                    }
                    global_charge(ast);
                }
                //local variable
                if (!strcmp(ast->name, "DefList"))
                {
                    local_charge(ast);
                }
                if (!strcmp(ast->name, "Exp"))
                {
                    check_exp_type(ast);

                    ast->next_layer = NULL;
                }
                if (!strcmp(ast->name, "RETURN"))
                {
                    func_return_type_compare(ast->next_neighbor);
                }
            }
            if (!strcmp(ast->name, "TYPE") ||
                !strcmp(ast->name, "ID") ||
                !strcmp(ast->name, "INT") ||
                !strcmp(ast->name, "FLOAT") ||
                !strcmp(ast->name, "CHAR"))
            {
                // printf(": %s", ast->value);
            }
            if (!strcmp(ast->name, "Program") ||
                !strcmp(ast->name, "Specifier") ||
                !strcmp(ast->name, "ExtDef") ||
                !strcmp(ast->name, "Dec") ||
                !strcmp(ast->name, "StmtList") ||
                !strcmp(ast->name, "ExtDecList") ||
                !strcmp(ast->name, "StructSpecifier") ||
                !strcmp(ast->name, "VarDec") || //TODO check define variable name
                !strcmp(ast->name, "FunDec") || //TODO check define function, check return type
                !strcmp(ast->name, "Exp") ||
                !strcmp(ast->name, "VarList") ||
                !strcmp(ast->name, "ParamDec") ||
                !strcmp(ast->name, "CompSt") ||
                !strcmp(ast->name, "ExtDefList") ||
                !strcmp(ast->name, "Stmt") ||
                !strcmp(ast->name, "DefList") ||
                !strcmp(ast->name, "Def") ||
                !strcmp(ast->name, "DecList") ||
                !strcmp(ast->name, "Args"))
            {
            }
        }

        struct ast *son = ast->next_layer;
        while (son != NULL)
        {
            parsetree(son, level + 1);
            son = son->next_neighbor;
        }
        if (scope)
        { //delete the last node
            variable_delete(0);
            scope = 0;
        }
    }
}

char *get_type(struct ast *ast)
{
    struct ast *child = ast->next_layer;
    char *type = "null_false";
    if (!strcmp(child->name, "LP"))
    {
        return get_type(child->next_neighbor);
    }
    else
    {
        if (child->next_neighbor != NULL)
        {
            if (!strcmp(child->next_neighbor->name, "OR") ||
                !strcmp(child->next_neighbor->name, "AND") ||
                !strcmp(child->next_neighbor->name, "LT") ||
                !strcmp(child->next_neighbor->name, "LE") ||
                !strcmp(child->next_neighbor->name, "GT") ||
                !strcmp(child->next_neighbor->name, "GE") ||
                !strcmp(child->next_neighbor->name, "NE") ||
                !strcmp(child->next_neighbor->name, "EQ"))
            {
                char *type_left = get_type(child);
                char *type_right = get_type(child->next_neighbor->next_neighbor);
                if (!strcmp(type_left, "null_false") ||
                    !strcmp(type_right, "null_false"))
                {
                }
                else if (strcmp(type_left, "int") || strcmp(type_right, "int"))
                {
                    printf("Error type 7 at Line %d: Unmatching operands\n", child->lineno);
                    continue_parse = 0;
                }
                else
                {
                    type = "boolean";
                }
            }
            else if (!strcmp(child->next_neighbor->name, "PLUS") ||
                     !strcmp(child->next_neighbor->name, "MINUS") ||
                     !strcmp(child->next_neighbor->name, "MUL") ||
                     !strcmp(child->next_neighbor->name, "DIV"))
            {
                char *type1 = get_type(child);
                char *type2 = get_type(child->next_neighbor->next_neighbor);
                if (!strcmp(type1, "null_false") ||
                    !strcmp(type2, "null_false"))
                {
                }
                else if (!((!strcmp(type1, "int") || !strcmp(type1, "float")) &&
                           (!strcmp(type2, "int") || !strcmp(type2, "float"))))
                {
                    printf("Error type 7 at Line %d: Unmatching operands.\n", child->lineno);
                }
                else
                {

                    if (!strcmp(type1, "float") ||
                        !strcmp(type2, "float"))
                    {
                        type = "float";
                    }
                    else
                    {
                        type = "int";
                    }
                }
            }
            else if (!strcmp(child->name, "NOT"))
            {
                char *type1 = get_type(child->next_neighbor);
                if (!strcmp(type1, "null_false"))
                {
                }
                else if (strcmp(type1, "int"))
                {
                    printf("Error type 7 ar Line %d: Unmatching operands.\n", child->lineno);
                }
                else
                {
                    type = "boolean";
                }
            }
            else if (!strcmp(child->name, "MINUS"))
            {
                char *type1 = get_type(child->next_neighbor);
                if (!strcmp(type1, "null_false"))
                {
                }
                else if (strcmp(type1, "int") && strcmp(type1, "float"))
                {
                    printf("Error type 7 at Line %d: Unmatching operands.\n", child->lineno);
                }
                else
                {
                    type = get_type(child->next_neighbor);
                }
            }
            else if (!strcmp(child->next_neighbor->name, "LP"))
            { //function
                char *func_name = child->value;
                VAL_FUNTION func_value = function_symtab_lookup(function_head, func_name);
                type = func_value.return_type;
                if (!strcmp(type, "null_false"))
                {
                    VAL_VARIABLE variable_value = variable_symtab_lookup(variable_head->symtab_variable, func_name);
                    if (strcmp(variable_value.type, "null_false"))
                    {
                        printf("Error type 11 at Line %d: invoking non-function variable: %s.\n", child->lineno, func_name);
                    }
                    else
                    {
                        printf("Error type 2 at Line %d: function %s has not been defined.\n", child->lineno, func_name);
                    }
                    continue_parse = 0;
                }
            }
            else if (!strcmp(child->next_neighbor->name, "LB"))
            { //array

                //FIXME
                struct ast *tmp_exp = child;

                if (!strcmp(tmp_exp->next_neighbor->name, "LB") &&
                    strcmp(get_type(tmp_exp->next_neighbor->next_neighbor), "int"))
                {
                    printf("Error type 12 at line %d: not int for index \n", tmp_exp->next_neighbor->next_neighbor->next_layer->lineno);
                }
                int dim = 1;

                while (tmp_exp->next_layer->next_neighbor != NULL && !strcmp(tmp_exp->next_layer->next_neighbor->name, "LB"))
                {
                    dim++;
                    if (strcmp(get_type(tmp_exp->next_neighbor->next_neighbor), "int"))
                    {
                        printf("Error type 12 at line %d: not int for index\n", tmp_exp->next_neighbor->next_neighbor->next_layer->lineno);
                    }
                    tmp_exp = tmp_exp->next_layer;
                }

                char *name = tmp_exp->next_layer->value;
                //******************
                int dim_has = array_check(child);
                if (dim_has > 0 && dim_has < dim)
                {
                    printf("Error type 10 at Line %d: for applying high dimention [] on non-array object.\n", child->lineno);
                }
                else if (dim_has <= 0)
                {
                    printf("Error type 10 at Line %d: for applying [] on non-array object on line.\n", child->lineno);
                }
                type = get_type(child);
                //******************
            }
            else if (!strcmp(child->next_neighbor->name, "DOT"))
            { //struct
                char *struct_type = get_type(child);
                char *field_name = child->next_neighbor->next_neighbor->value;
                if (!strcmp(struct_type, "int") ||
                    !strcmp(struct_type, "float") ||
                    !strcmp(struct_type, "char") ||
                    !strcmp(struct_type, "boolean"))
                {
                    printf("Error type 13 at Line %d: non-struct object access dot\n", child->lineno);
                }
                else
                {
                    VAL_VARIABLE struct_field = get_variable_value(struct_type);
                    VAL_VARIABLE value = get_struct_field_value(struct_field.field, field_name);
                    if (!strcmp(value.type, "null_false"))
                    {
                        printf("Error type 14 at Line %d: no such member: %s\n", child->lineno, field_name);
                    }

                    type = value.type;
                }
            }
        }
        else
        {
            if (!strcmp(child->name, "ID"))
            { //variable
                type = get_variable_value(child->value).type;
                if (!strcmp(type, "null_false"))
                {
                    printf("Error type 1 at Line %d: variable %s is not defined before.\n", child->lineno, child->value);
                    continue_parse = 0;
                }
                // printf("%s\n", type);
            }
            else
            {
                if (!strcmp(child->name, "INT"))
                {
                    type = "int";
                }
                else if (!strcmp(child->name, "FLOAT"))
                {

                    type = "float";
                }
                else if (!strcmp(child->name, "CHAR"))
                {
                    type = "char";
                }
            }
        }
    }

    return type;
}

int check_exp_type(struct ast *exp) // check exp
{

    struct ast *child_layer = exp->next_layer;
    struct ast *neighbor = child_layer->next_neighbor;
    if (neighbor == NULL)
    {
        char *name = child_layer->value;
        if (!strcmp(child_layer->name, "ID"))
        { //variable
            struct variable_list *tmp_head = variable_head;
            int undefine_ = 1;
            while (tmp_head->parent != NULL)
            {

                VAL_VARIABLE a = variable_symtab_lookup(tmp_head->symtab_variable, name);

                if (strcmp(a.type, "null_false"))
                {
                    undefine_ = 0;
                    break;
                }
                tmp_head = tmp_head->parent;
            }
            if (undefine_)
            {
                printf("Error type 1 at Line %d: variable %s is not define before\n", child_layer->lineno, name);
            }
        }
        else
        {
        }

        return 0;
    }
    else
    {
        if (!strcmp(neighbor->name, "ASSIGN"))
        {

            struct ast *tmp_child = child_layer;
            while (tmp_child->next_layer->next_layer != NULL)
            {
                tmp_child = tmp_child->next_layer;
            }

            if (strcmp(tmp_child->next_layer->name, "ID"))
            {
                printf("Error type 6 at Line %d: rvalue on the left side of assignment operator on line.\n", child_layer->next_layer->lineno);
                continue_parse = 0;
            }
            else
            {

                char *left_type = get_type(child_layer);

                char *right_type = get_type(neighbor->next_neighbor);
                // printf("%s, %s, %d\n", left_type, right_type, tmp_child->lineno);

                if (!strcmp(left_type, "null_false") ||
                    !strcmp(right_type, "null_false"))
                {
                    return -1;
                }

                //variable
                struct ast *exp_tmp = child_layer;
                while (strcmp(exp_tmp->next_layer->name, "ID"))
                {
                    exp_tmp = exp_tmp->next_layer;
                }

                char *name = exp_tmp->next_layer->value;

                struct variable_list *tmp_head = variable_head;
                int undefine_ = 1;
                VAL_VARIABLE a;
                while (tmp_head->parent != NULL)
                {

                    a = variable_symtab_lookup(tmp_head->symtab_variable, name);

                    if (strcmp(a.type, "null_false"))
                    {
                        undefine_ = 0;
                        break;
                    }
                    tmp_head = tmp_head->parent;
                }

                if (undefine_)
                {
                    printf("Error type 1 at Line %d: variable %s is not defined before.\n", child_layer->lineno, name);
                }
                else
                { //define before

                    // char *a_type = get_type(child_layer);
                    // char *right_type = get_type(neighbor->next_neighbor);
                    if (strcmp(left_type, right_type))
                    {
                        // printf("%s, %s\n", left_type, right_type);
                        printf("Error type 5 at Line%d: Unmatching type on both sides of assignment\n", neighbor->next_neighbor->lineno);
                        continue_parse = 0;
                    }
                }
            }
        }
        else if (!strcmp(neighbor->name, "PLUS") ||
                 !strcmp(neighbor->name, "MINUS") ||
                 !strcmp(neighbor->name, "MUL") ||
                 !strcmp(neighbor->name, "DIV"))
        {
        }
        //bool
        else if (!strcmp(neighbor->name, "AND") ||
                 !strcmp(neighbor->name, "OR") ||
                 !strcmp(neighbor->name, "LT") ||
                 !strcmp(neighbor->name, "LE") ||
                 !strcmp(neighbor->name, "GT") ||
                 !strcmp(neighbor->name, "GE") ||
                 !strcmp(neighbor->name, "NE") ||
                 !strcmp(neighbor->name, "EQ"))
        {
        }
    }
    if (!strcmp(child_layer->name, "MINUS"))
    {
        return 0;
    }
    else if (!strcmp(child_layer->name, "NOT"))
    {
        //check bool
        return 3;
    }
    else if (!strcmp(child_layer->name, "ID"))
    { //function check
        char *func_name = child_layer->value;
        VAL_FUNTION look_up = function_symtab_lookup(function_head, func_name);

        if (!strcmp(look_up.return_type, "null_false"))
        { //illegal function
            struct variable_list *tmp_head = variable_head;
            int undefine_ = 1;
            while (tmp_head->parent != NULL)
            {

                VAL_VARIABLE a = variable_symtab_lookup(tmp_head->symtab_variable, func_name);

                if (strcmp(a.type, "null_false"))
                {
                    undefine_ = 0;
                    printf("Error type 11 at Line %d:miss using variable %s as function\n", child_layer->lineno, func_name);
                    break;
                }
                tmp_head = tmp_head->parent;
            }
            if (undefine_)
            {
                printf("Error type 2 at Line %d: function %s is not defined\n", exp->lineno, func_name);
            }
        }
        else
        { //legal function
            struct ast *args_tmp = NULL;
            if (strcmp(child_layer->next_neighbor->next_neighbor->name, "RP"))
            {
                args_tmp = child_layer->next_neighbor->next_neighbor;
            }
            int com_res = func_args_compare(args_tmp, look_up.parameters);
            if (!com_res)
            {
                printf("Error type 9 at Line %d: arguments mismatch.\n", child_layer->next_neighbor->next_neighbor->lineno);
            }
        }
    }
    else if (!strcmp(neighbor->name, "LB"))
    {
        //check array, error 10, 12
    }
    else if (!strcmp(neighbor->name, "DOT"))
    {
        //check struct, error 13, 14, 15
    }

    return -1;
};

//scope variable insert
void variable_insert()
{
    symtab_variable *symtab_variable = variable_symtab_init();

    struct variable_list *variable_head_tmp = (struct variable_list *)malloc(sizeof(struct variable_list));
    memset(variable_head_tmp, '\0', sizeof(struct variable_list));

    variable_head_tmp->symtab_variable = symtab_variable;

    variable_head_tmp->parent = variable_head;

    variable_head->childe = variable_head_tmp;

    variable_head = variable_head->childe;
}

void variable_delete(int is_struct)
{
    variable_head = variable_head->parent;
    if (!is_struct)
    {
        free(variable_head->childe);
    }

    variable_head->childe = NULL;
}

void global_charge(struct ast *ast)
{
    struct ast *specifier = ast->next_layer;
    if (!strcmp(specifier->next_layer->name, "TYPE")) //variable or function
    {

        if (!strcmp(specifier->next_neighbor->name, "FunDec")) //function
        {

            VAL_FUNTION function;
            function.return_type = specifier->next_layer->value;
            if (strcmp(specifier->next_neighbor->next_layer->next_neighbor->next_neighbor->name, "RP"))
            {
                function.parameters = specifier->next_neighbor->next_layer->next_neighbor->next_neighbor;
                func_args_charge(function.parameters);
            }
            else
            {
                function.parameters = NULL;
            }
            char *name = specifier->next_neighbor->next_layer->value;
            int insert_res = function_symtab_insert(function_head, name, function);
            if (!insert_res)
            {
                printf("Error type 4 at Line %d: function %s has been defined before.\n", specifier->lineno, name);
                continue_parse = 0;
            }

            ast->next_layer = specifier->next_neighbor->next_neighbor;
        }
        else
        { //variable
            VAL_VARIABLE *value = malloc(sizeof(VAL_VARIABLE));
            memset(value, '\0', sizeof(VAL_VARIABLE));
            value->type = specifier->next_layer->value;
        }
    }
    else // struct
    {

        struct ast *struct_layer = specifier->next_layer->next_layer;
        if (struct_layer->next_neighbor->next_neighbor != NULL)
        { //define struct
            char *struct_name = struct_layer->next_neighbor->value;
            int define = variable_check_define(struct_name);
            if (define)
            {
                printf("Error type 15 at Line%d: redefine struct: %s\n", struct_layer->lineno, struct_name);
            }
            else
            {
                VAL_VARIABLE value;
                value.type = struct_name;
                variable_symtab_insert(variable_head->symtab_variable, struct_name, value);
                struct ast *def_list = struct_layer->next_neighbor->next_neighbor->next_neighbor;
                variable_insert();
                local_charge(def_list);
                value.field = variable_head->symtab_variable;
                variable_delete(1);
                variable_head->symtab_variable->entry.value = value;
            }
        }
    }
}

void func_args_charge(struct ast *var_list)
{
    struct ast *param_dec = var_list->next_layer;
    char *type = "";
    if (!strcmp(param_dec->next_layer->next_layer->name, "TYPE")) //not strut
    {
        type = param_dec->next_layer->next_layer->value;
    }
    else
    { //struct
        char *type = param_dec->next_layer->next_layer->next_layer->next_neighbor->value;
        int define = variable_check_define_global(type);

        if (!define)
        {
            printf("error type 14 for undefine struct in line %d\n", param_dec->next_layer->next_layer->lineno);
        }
    }
    var_dec_charge(type, param_dec->next_layer->next_neighbor);

    if (var_list->next_layer->next_neighbor != NULL)
    {
        func_args_charge(var_list->next_layer->next_neighbor->next_neighbor);
    }
}

void local_charge(struct ast *ast)
{
    // printf("%s, %d\n", ast->name, ast->lineno);

    if (ast->next_layer == NULL)
    { //struct for first line, hanle it before
        return;
    }

    struct ast *def = ast->next_layer;
    struct ast *specifier = def->next_layer;
    if (!strcmp(specifier->next_layer->name, "TYPE"))
    { //variable
        char *type = specifier->next_layer->value;
        dec_list_charge(type, specifier->next_neighbor);
    }
    else
    {
        char *type = specifier->next_layer->next_layer->next_neighbor->value;
        int define = variable_check_define(type);
        if (!define)
        {
            printf("Error type 14 at Line %d: Undefine struct %s\n", specifier->next_layer->lineno, type);
        }

        dec_list_charge(type, specifier->next_neighbor);
    }

    if (def->next_neighbor->name != NULL)
    {

        // ast->next_layer = def->next_neighbor;
        local_charge(def->next_neighbor);
    }

    ast->next_layer = NULL;
}

void dec_list_charge(char *type, struct ast *ast)
{
    // printf("%s, %d\n", ast->name, ast->lineno);
    struct ast *dec = ast->next_layer;
    dec_charge(type, dec);

    if (dec->next_neighbor != NULL)
    {
        ast = dec->next_neighbor->next_neighbor;
        dec_list_charge(type, ast);
    }
}

void dec_charge(char *type, struct ast *ast)
{
    int i;
    i = var_dec_charge(type, ast->next_layer);

    if (i >= 0 && ast->next_layer->next_neighbor != NULL)
    {
        //assign

        check_exp_type(ast->next_layer->next_neighbor->next_neighbor);
        char *right_type = get_type(ast->next_layer->next_neighbor->next_neighbor);
        if (!strcmp(right_type, "null_false"))
        {
        }
        else if (strcmp(type, right_type))
        {
            printf("type 5 error on line %d for unmatching type\n", ast->next_layer->next_neighbor->next_neighbor->lineno);
        }
    }
}

int var_dec_charge(char *type, struct ast *ast)
{
    // printf("%s, %d\n", ast->name, ast->lineno);
    int i;
    i = 0;
    char *name;
    if (!strcmp(ast->next_layer->name, "ID"))
    {
        name = ast->next_layer->value;
    }
    else
    {
        struct ast *tmp_var_dec = ast;
        while (!strcmp(tmp_var_dec->next_layer->name, "VarDec"))
        {
            i++;
            tmp_var_dec = tmp_var_dec->next_layer;
        }
        name = tmp_var_dec->next_layer->value;
    }

    struct variable_list *tmp_head = variable_head;
    // printf("%d\n", variable_head->parent->parent != NULL);
    while (tmp_head->parent != NULL)
    {

        VAL_VARIABLE a = variable_symtab_lookup(tmp_head->symtab_variable, name);

        if (strcmp(a.type, "null_false"))
        {
            // printf("duplicate variable %s in line %d\n", name, ast->lineno);
            printf("Error type 3 at Line %d: variable %s has been defined in this scope.\n", ast->lineno, name);
            continue_parse = 0;

            i = -1;
            break;
        }
        tmp_head = tmp_head->parent;
    }
    if (i >= 0)
    {
        VAL_VARIABLE value;

        value.type = type;
        value.dim = i;
        variable_symtab_insert(variable_head->symtab_variable, name, value);
    }

    return i;
}

void scan_tree(struct ast *ast, int level)
{
    if (ast != NULL)
    {
        if (ast->name != NULL)
        {
            for (int i = 0; i < level; i++)
            {
                printf("  ");
            }
            if (ast->lineno != -1)
            {
                //check assign, function need return type, lack of specifier type.
                //check operation of unsuitable operation
                printf("%s", ast->name);
            }
            if (!strcmp(ast->name, "TYPE") ||
                !strcmp(ast->name, "ID") ||
                !strcmp(ast->name, "INT") ||
                !strcmp(ast->name, "FLOAT") ||
                !strcmp(ast->name, "CHAR"))
            {
                printf(": %s", ast->value);
            }
            if (!strcmp(ast->name, "Program") ||
                !strcmp(ast->name, "Specifier") ||
                !strcmp(ast->name, "ExtDef") ||
                !strcmp(ast->name, "Dec") ||
                !strcmp(ast->name, "StmtList") ||
                !strcmp(ast->name, "ExtDecList") ||
                !strcmp(ast->name, "StructSpecifier") ||
                !strcmp(ast->name, "VarDec") || //TODO check define variable name
                !strcmp(ast->name, "FunDec") || //TODO check define function, check return type
                !strcmp(ast->name, "Exp") ||
                !strcmp(ast->name, "VarList") ||
                !strcmp(ast->name, "ParamDec") ||
                !strcmp(ast->name, "CompSt") ||
                !strcmp(ast->name, "ExtDefList") ||
                !strcmp(ast->name, "Stmt") ||
                !strcmp(ast->name, "DefList") ||
                !strcmp(ast->name, "Def") ||
                !strcmp(ast->name, "DecList") ||
                !strcmp(ast->name, "Args"))
            {
                printf(" (%d)", ast->lineno);
            }
            printf("\n");
        }

        struct ast *son = ast->next_layer;
        while (son != NULL)
        {
            // printf("%s\n", "hello");
            scan_tree(son, level + 1);
            son = son->next_neighbor;
        }
    }
}

int get_args_num(struct ast *var_list)
{
    int i = 0;
    struct ast *var_tmp = var_list;
    while (var_tmp->next_layer->next_neighbor != NULL)
    {
        i++;
        var_tmp = var_tmp->next_layer->next_neighbor->next_neighbor;
    }
    return i;
}

void func_return_type_compare(struct ast *ast)
{

    struct symtab_function *function_tmp = function_head;
    while (function_tmp->next->next != NULL)
    {
        function_tmp = function_tmp->next;
    }

    VAL_FUNTION function = function_tmp->entry.value;

    char *return_type = get_type(ast);
    if (!strcmp(return_type, "null_false"))
    {
    }
    else if (strcmp(function.return_type, return_type))
    {
        printf("Error type 8 at line %d: Unmatching return type.\n", ast->lineno);
        continue_parse = 0;
    }
}

//args1: tmp, args2:require
int func_args_compare(struct ast *args1, struct ast *args2)
{
    if ((args1 == NULL && args2 != NULL) ||
        (args1 != NULL && args2 == NULL))

    {
        return 0;
    }

    //number
    int args1_num = get_args_num(args1);
    int args2_num = get_args_num(args2);

    if (args2_num != args1_num)
    {
        return 0;
    }

    //type
    struct ast *var_tmp_1 = args1;
    struct ast *var_tmp_2 = args2;

    while (var_tmp_1->next_layer->next_neighbor != NULL)
    {
        char *tmp_type = get_type(var_tmp_1->next_layer);
        char *param_type = var_tmp_2->next_layer->next_layer->next_layer->value;
        if (strcmp(tmp_type, param_type))
        {
            return 0;
        }

        var_tmp_1 = var_tmp_1->next_layer->next_neighbor->next_neighbor;
        var_tmp_2 = var_tmp_2->next_layer->next_neighbor->next_neighbor;
    }
    return 1;
}

VAL_VARIABLE get_variable_value(char *name)
{

    struct variable_list *tmp_head = variable_head;
    VAL_VARIABLE a = variable_symtab_lookup(tmp_head->symtab_variable, name);
    while (tmp_head->parent != NULL)
    {

        a = variable_symtab_lookup(tmp_head->symtab_variable, name);

        if (strcmp(a.type, "null_false"))
        { //define before
            // printf("%s\n", a.type);
            return a;
        }
        tmp_head = tmp_head->parent;
    }

    return a;
}

VAL_VARIABLE get_struct_field_value(struct symtab_variable *struct_field, char *name)
{
    VAL_VARIABLE value = variable_symtab_lookup(struct_field, name);
    return value;
}

int variable_check_define(char *name)
{
    struct variable_list *tmp_head = variable_head;
    while (tmp_head->parent != NULL)
    {
        VAL_VARIABLE a = variable_symtab_lookup(tmp_head->symtab_variable, name);

        if (strcmp(a.type, "null_false"))
        { //define before
            return 1;
        }
        tmp_head = tmp_head->parent;
    }

    return 0;
}

int variable_check_define_global(char *name)
{
    struct variable_list *tmp_head = variable_head;
    while (tmp_head->parent->parent != NULL)
    {
        tmp_head = tmp_head->parent;
    }

    // symtab_variable *ptr = tmp_head->symtab_variable;
    // while (ptr->next != NULL)
    // {
    //     printf("%s, ", ptr->entry.key);
    //     ptr = ptr->next;
    // }
    // printf("\n");

    // print_variable_head();
    // printf("%s\n", name);
    // printf("%s\n",tmp_head->symtab_variable->next->entry.key);
    // printf("\n");

    VAL_VARIABLE a = variable_symtab_lookup(tmp_head->symtab_variable, name);

    if (strcmp(a.type, "null_false"))
    { //define before
        return 1;
    }
    return 0;
}

int semantic(struct ast *ast, int level)
{

    variable_head = malloc(sizeof(struct variable_list));
    memset(variable_head, '\0', sizeof(struct variable_list));
    variable_head->parent = NULL;

    function_head = function_symtab_init(); //function list

    parsetree(ast, level);
    // scan_tree(ast, level);
    return 0;
}
