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

void scan_tree(struct ast *ast, int level);
void variable_insert();
void global_charge(struct ast *ast); //function struct define
void local_charge(struct ast *ast);  //variable insert
void dec_list_charge(char *type, struct ast *ast);
void dec_charge(char *type, struct ast *ast);
int var_dec_charge(char *type, struct ast *ast);
void variable_delete(int is_struct);
VAL_VARIABLE get_variable_value(char *name);
VAL_VARIABLE get_struct_field_value(struct symtab_variable *struct_field, char *name);

void parsetree(struct ast *ast, int level)
{
    int scope = 0;
    if (ast != NULL)
    {
        if (ast->name != NULL)
        {
            if (ast->lineno != -1)
            {
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
                }
                if (!strcmp(ast->name, "RETURN"))
                {
                }
            }
            if (!strcmp(ast->name, "TYPE") ||
                !strcmp(ast->name, "ID") ||
                !strcmp(ast->name, "INT") ||
                !strcmp(ast->name, "FLOAT") ||
                !strcmp(ast->name, "CHAR"))
            {
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
            }
            else
            {
                function.parameters = NULL;
            }
            char *name = specifier->next_neighbor->next_layer->value;
            int insert_res = function_symtab_insert(function_head, name, function);
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

void local_charge(struct ast *ast)
{

    if (ast->next_layer == NULL)
    { //struct for first line, handle it before
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
        dec_list_charge(type, specifier->next_neighbor);
    }

    if (def->next_neighbor->name != NULL)
    {
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

    if (i >= 0)
    { //array
    }
}

int var_dec_charge(char *type, struct ast *ast)
{
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

    VAL_VARIABLE value;
    value.type = type;
    value.dim = i;
    variable_symtab_insert(variable_head->symtab_variable, name, value);

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
            scan_tree(son, level + 1);
            son = son->next_neighbor;
        }
    }
}

VAL_VARIABLE get_variable_value(char *name)
{

    struct variable_list *tmp_head = variable_head;
    VAL_VARIABLE a = variable_symtab_lookup(tmp_head->symtab_variable, name);
    while (tmp_head->parent != NULL)
    {

        a = variable_symtab_lookup(tmp_head->symtab_variable, name);

        if (strcmp(a.type, "null_false"))
        {
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
