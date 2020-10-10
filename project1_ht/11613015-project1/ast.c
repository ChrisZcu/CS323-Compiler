#ifndef STDARG_H
#define STDARG_H
#include <stdarg.h>
#endif

#ifndef LEX_YY_C
#define LEX_YY_C
#include "lex.yy.c"
#endif

#ifndef STDARG_H
#define STDARG_H
#include <stdarg.h>
#endif


struct ast {
    int lineno;
    char *name;
    char *value;

    struct ast *sub_ast;
    struct ast *next_sibling;
};



struct ast *new_ast(char *name, int num, ...) {
    va_list valist;
    struct ast *node = (struct ast*)malloc(sizeof(struct ast));
    struct ast *tmp = (struct ast*)malloc(sizeof(struct ast));
    node->name = name;

    va_start(valist, num);

    if (num > 0) {
        tmp = va_arg(valist, struct ast*);
        node->sub_ast = tmp;
        node->lineno = tmp->lineno;
        num --;
        while (num > 0) {
            tmp->next_sibling = va_arg(valist, struct ast*);
            tmp = tmp->next_sibling;
            num--;
        }
    }

    return node;
}


struct ast *new_node(char *name, char *value, int lineno) {
    struct ast *node = (struct ast*)malloc(sizeof(struct ast));
    node->name = name;
    node->value = value;
    node->lineno = lineno;
}


void preorder(struct ast *ast, int level) {
    if (ast != NULL) {
        if(ast->name != NULL) {
            for(int i=0; i<level; i++) {
                printf("  ");
            }
            if(ast->lineno != -1) {
                printf ("%s", ast->name);
            }
            if(!strcmp(ast->name, "TYPE") ||
                    !strcmp(ast->name, "ID") ||
                    !strcmp(ast->name, "INT") ||
                    !strcmp(ast->name, "FLOAT") ||
                    !strcmp(ast->name, "CHAR")) {
                printf(": %s", ast->value);
            }
            if(!strcmp(ast->name, "Program") ||
                    !strcmp(ast->name, "ExtDefList") ||
                    !strcmp(ast->name, "ExtDef") ||
                    !strcmp(ast->name, "ExtDecList") ||
                    !strcmp(ast->name, "Specifier") ||
                    !strcmp(ast->name, "StructSpecifier") ||
                    !strcmp(ast->name, "VarDec") ||
                    !strcmp(ast->name, "FunDec") ||
                    !strcmp(ast->name, "VarList") ||
                    !strcmp(ast->name, "ParamDec") ||
                    !strcmp(ast->name, "CompSt") ||
                    !strcmp(ast->name, "StmtList") ||
                    !strcmp(ast->name, "Stmt") ||
                    !strcmp(ast->name, "DefList") ||
                    !strcmp(ast->name, "Def") ||
                    !strcmp(ast->name, "DecList") ||
                    !strcmp(ast->name, "Dec") ||
                    !strcmp(ast->name, "Exp") ||
                    !strcmp(ast->name, "Args")) {
                printf(" (%d)", ast->lineno);
            }

            printf("\n");
        }
        
        struct ast *each_son = ast->sub_ast;
        while (each_son != NULL) {
            preorder(each_son, level+1);
            each_son = each_son->next_sibling;
        }

    }
}



