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

int check_exp_type(struct ast *ast);
void scan_tree(struct ast *ast, int level);
void variable_insert();
void global_charge(struct ast *ast);
void local_charge(struct ast *ast);
void dec_list_charge(char *type, struct ast *ast);
void dec_charge(char *type, struct ast *ast);
int var_dec_charge(char *type, struct ast *ast);
int func_args_compare(struct ast *args1, struct ast *args2);
//TODO check the semantic
void parsetree(struct ast *ast, int level)
{
    int scope = 0;
    if (ast != NULL)
    {
        if (ast->name != NULL)
        {
            if (ast->lineno != -1)
            {
                //check assign, function need return type, lack of specifier type.
                //check operation of unsuitable operation

                if (!strcmp(ast->name, "CompSt"))
                {
                    scope = 1;
                    // add new scope
                    variable_insert();
                }
                //global variable
                if (!strcmp(ast->name, "ExtDef"))
                {

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

            variable_head = variable_head->parent;

            free(variable_head->childe);

            variable_head->childe = NULL;
        }
    }
}

char *get_type(struct ast *ast)
{
    struct ast *child = ast->next_layer;
    char *type = "";
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

                if (strcmp(type_left, "int") || strcmp(type_right, "int"))
                {
                    printf("error type 7 for unmatching operands in line %d\n", child->lineno);
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

                if (!((!strcmp(type1, "int") || !strcmp(type1, "float")) &&
                      (!strcmp(type2, "int") || !strcmp(type2, "float"))))
                {
                    printf("error type 7 for unmatching operands in line %d\n", child->lineno);
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
                if (strcmp(type1, "int"))
                {
                    printf("error type 7 for unmatching operands in line %d\n", child->lineno);
                }
                else
                {

                    type = "boolean";
                }
            }
            else if (!strcmp(child->name, "MINUS"))
            {
                char *type1 = get_type(child->next_neighbor);
                if (strcmp(type1, "int") && strcmp(type1, "float"))
                {
                    printf("error type 7 for unmatching operands in line %d\n", child->lineno);
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
            }
            else if (!strcmp(child->next_neighbor->name, "LB"))
            { //array
                if (strcmp(child->next_neighbor->next_neighbor->next_layer->name, "INT"))
                {
                    printf("type 12 error on line %d\n", child->next_neighbor->next_neighbor->next_layer->lineno);
                }
            }
            else if (!strcmp(child->next_neighbor->name, "DOT"))
            { //struct
            }
        }
        else
        {

            if (!strcmp(child->name, "ID"))
            { //variable
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
                printf("variable %s not define before in line %d\n", name, child_layer->lineno);
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
            if (strcmp(child_layer->next_layer->name, "ID"))
            {
                printf("Error type 6: rvalue on the left side of assignment operator on line %d\n", child_layer->next_layer->lineno);
            }
            else
            {
                //variable
                char *name = child_layer->next_layer->value;
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
                    printf("variable %s is not defined before in line %d\n", name, child_layer->lineno);
                }
                else
                { //define before
                    char *a_type = a.type;
                    char *right_type = get_type(neighbor->next_neighbor);

                    if (strcmp(a_type, right_type))
                    {
                        printf("type 5 error on line %d for unmatching type\n", neighbor->next_neighbor->lineno);
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
                    printf("miss using variable %s as function in line %d\n", func_name, child_layer->lineno);
                    break;
                }
                tmp_head = tmp_head->parent;
            }
            if (undefine_)
            {
                printf("undefine function %s in line %d\n", func_name, exp->lineno);
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
                printf("error type 9 for arguments mismatch in line %d\n", child_layer->next_neighbor->next_neighbor->lineno);
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

void global_charge(struct ast *ast)
{
    struct ast *specifier = ast->next_layer;
    // int exp_type = check_exp_type(specifier);
    if (!strcmp(specifier->next_layer->name, "TYPE")) //variable or function
    {

        if (!strcmp(specifier->next_neighbor->name, "FunDec")) //function
        {

            VAL_FUNTION function;
            function.return_type = specifier->next_layer->value;
            function.parameters = specifier->next_neighbor->next_layer->next_neighbor->next_neighbor;
            char *name = specifier->next_neighbor->next_layer->value;
            int insert_res = function_symtab_insert(function_head, name, function);
            if (!insert_res)
            {
                printf("duplicate function name %s in line %d\n", name, specifier->lineno);
            }
            // ast = specifier->next_neighbor->next_neighbor; //jump the function define head
            struct ast *stmt_tmp = specifier->next_neighbor->next_neighbor->next_layer->next_neighbor->next_neighbor;
            struct ast *stmt_parent = stmt_tmp;

            while (stmt_tmp->next_layer->next_neighbor->name != NULL)
            {
                stmt_parent = stmt_tmp;
                stmt_tmp = stmt_tmp->next_layer->next_neighbor;
            }

            if (strcmp(stmt_tmp->next_layer->next_layer->name, "RETURN"))
            {
                printf("missing return value on line %d\n", stmt_tmp->next_layer->next_layer->lineno);
                return;
            }

            char *return_type = get_type(stmt_tmp->next_layer->next_layer->next_neighbor);

            if (strcmp(function.return_type, return_type))
            {
                printf("error type 8 for unmatching return type on line %d\n", stmt_tmp->next_layer->next_layer->next_neighbor->lineno);
            }

            stmt_parent->next_layer->next_neighbor = NULL;

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
    }
}

void local_charge(struct ast *ast)
{
    struct ast *def = ast->next_layer;
    struct ast *specifier = def->next_layer;
    char *type = specifier->next_layer->value;

    dec_list_charge(type, specifier->next_neighbor);

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
        if (strcmp(type, right_type))
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
        while (!strcmp(tmp_var_dec->next_layer->name, "VarDex"))
        {
            i++;
            tmp_var_dec = tmp_var_dec->next_layer;
        }
        name = tmp_var_dec->value;
    }

    struct variable_list *tmp_head = variable_head;
    // printf("%d\n", variable_head->parent->parent != NULL);
    while (tmp_head->parent != NULL)
    {

        VAL_VARIABLE a = variable_symtab_lookup(tmp_head->symtab_variable, name);

        if (strcmp(a.type, "null_false"))
        {
            printf("duplicate variable %s in line %d\n", name, ast->lineno);
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
            printf("error type 9 for arguments mismatch in line %d\n", args1->lineno);
            break;
        }

        var_tmp_1 = var_tmp_1->next_layer->next_neighbor->next_neighbor;
        var_tmp_2 = var_tmp_2->next_layer->next_neighbor->next_neighbor;
    }
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
