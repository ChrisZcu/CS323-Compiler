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

//INT = 0, FLOAT = 1, STRUCT = 2, ARRAY = 3, BOOL = 4

int get_type(char *name)
{
    if (!strcmp(name, "INT"))
    {
        return 0;
    }
    else if (!strcmp(name, "FLOAT"))
    {
        return 1;
    }
    else if (!strcmp(name, "STRUCT"))
    {
        return 2;
    }
}

int check_exp_type(struct ast *exp) // check exp
{

    struct ast *child_layer = exp->next_layer;
    struct ast *neighbor = child_layer->next_neighbor;
    if (neighbor == NULL) //return type
    {
        return get_type(child_layer->name);
    }
    else
    {
        if (!strcmp(neighbor->name, "ASSIGN"))
        { //TODO check define, error 1
        }
        else if (!strcmp(neighbor->name, "PLUS") ||
                 !strcmp(neighbor->name, "MINUS") ||
                 !strcmp(neighbor->name, "MUL") ||
                 !strcmp(neighbor->name, "DIV"))
        {
            //check type, error 7
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
        return get_type(neighbor->name);
    }
    else if (!strcmp(child_layer->name, "NOT"))
    {
        //check bool
        return 3;
    }
    else if (!strcmp(child_layer->name, "ID"))
    { //function check, error 2
        char *func_name = child_layer->value;
        VAL_FUNTION look_up = function_symtab_lookup(function_head, func_name);

        if (!strcmp(look_up.return_type, "null_false"))
        {
            printf("undefine function %s in line %d\n", func_name, exp->lineno);
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
        // printf("%s, %d\n", ast->next_layer->next_neighbor->next_neighbor->name, ast->lineno);
        check_exp_type(ast->next_layer->next_neighbor->next_neighbor);
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
