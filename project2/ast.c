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

enum EXP_TYPE
{
    INT = 0,
    FLOAT = 1,
    STRUCT = 2,
    ARRAY = 3,
    BOOL = 4
} exp_type;

struct ast
{
    char *name;
    char *value;
    int lineno;

    struct ast *next_layer;
    struct ast *next_neighbor;
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

//TODO check the semantic
void parsetree(struct ast *ast, int level)
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
            parsetree(son, level + 1);
            son = son->next_neighbor;
        }
    }
}

int semantic(struct ast *ast, int level)
{
    symtab_variable *symtab_variable = variable_symtab_init();
    symtab_function *symtab_function = function_symtab_init();
    parsetree(ast, level);
    return 0;
}

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
