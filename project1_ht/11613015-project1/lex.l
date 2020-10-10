%{
    #include <string.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include "syntax.tab.h"
    #ifndef AST_C
    #define AST_C
    #include "ast.c"
    #endif

    int yycolumn = 1;
    extern int has_error;
    extern int has_lexical_error;
    #define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno; \
        yylloc.first_column = yycolumn; yylloc.last_column = yycolumn+yyleng-1; \
        yycolumn += yyleng;
%}
%option yylineno 
HEX_INT [-+]?0[xX]([0-9a-fA-F]{0,8})
OCT_INT [-+]?0([0-3]?[0-7]{0,10})
DEC_INT [-+]?0|([1-9][0-9]*)
HEX_INT_WRONG [-+]?0[xX][0-9a-zA-Z]*
OCT_INT_WRONG [-+]?0[a-wy-z][0-9a-zA-Z]*
INT {HEX_INT}|{OCT_INT}|{DEC_INT}
INT_WRONG {HEX_INT_WRONG}|{OCT_INT_WRONG}
HEX_FLOAT {HEX_INT}\.[0-9a-f]*
OCT_FLOAT {OCT_INT}\.[0-7]*
DEC_FLOAT [-+]?(0|[1-9][0-9]*)\.[0-9]*
FLOAT {HEX_FLOAT}|{OCT_FLOAT}|{DEC_FLOAT}
CHAR ('[a-zA-Z0-9]')|('\\x[0-9a-fA-F]{2}')
CHAR_WRONG ('\\x[0-9a-zA-Z]*')
ID ([a-zA-Z_][0-9a-zA-Z_]*)
ID_WRONG [0-9][0-9a-zA-Z_]*
DOT \.
SEMI ;
COMMA ,
ASSIGN =
LT <
LE <=
GT >
GE >=
NE !=
EQ ==
PLUS \+
MINUS -
MUL \*
DIV \/
AND &&
OR \|\|
NOT !
LP \(
RP \)
LB \[
RB \]
LC \{
RC \}
%%
{FLOAT} { 
    yylval.ast = new_node("FLOAT", strdup(yytext), yylineno);
    return FLOAT;
}
{INT} {
    yylval.ast = new_node("INT", strdup(yytext), yylineno);
    return INT;
}
{INT_WRONG} {
    printf("Error type A at Line %d: Wrong Int Form \'%s\'\n", 
    yylineno, yytext);
    has_lexical_error = 1;
    has_error = 1;
}
{CHAR} {
    yylval.ast = new_node("CHAR", strdup(yytext), yylineno);
    return CHAR;    
}
{CHAR_WRONG} {
    printf("Error type A at Line %d: Wrong Char Form \'%s\'\n", 
    yylineno, yytext);
    has_lexical_error = 1;
    has_error = 1;
}
{ID} {
    if (strcmp(yytext, "int") == 0 ||
            strcmp(yytext, "float") == 0 ||
            strcmp(yytext, "char") == 0) {
        yylval.ast = new_node("TYPE", strdup(yytext), yylineno);
        return TYPE;
    }    
    else if (strcmp(yytext, "struct") == 0) {
        yylval.ast = new_node("STRUCT", NULL, yylineno);
        return STRUCT;
    }
    else if (strcmp(yytext, "if") == 0) {
        yylval.ast = new_node("IF", NULL, yylineno);
        return IF;
    }
    else if (strcmp(yytext, "else") == 0) {
        yylval.ast = new_node("ELSE", NULL, yylineno);
        return ELSE;
    }
    else if (strcmp(yytext, "while") == 0) {
        yylval.ast = new_node("WHILE", NULL, yylineno);
        return WHILE;
    }
    else if (strcmp(yytext, "return") == 0) {
        yylval.ast = new_node("RETURN", NULL, yylineno);
        return RETURN;
    }
    else {
        yylval.ast = new_node("ID", strdup(yytext), yylineno);
        return ID;
    }
}
{ID_WRONG} {
    printf("Error type A at Line %d: Wrong Form \"%s\"\n", 
    yylineno, yytext);
    has_lexical_error = 1;
    has_error = 1;
}
{DOT} {
    yylval.ast = new_node("DOT", NULL, yylineno);
    return DOT;
}
{SEMI} { 
    yylval.ast = new_node("SEMI", NULL, yylineno);
    return SEMI;    
}
{COMMA} { 
    yylval.ast = new_node("COMMA", NULL, yylineno);
    return COMMA;
}
{ASSIGN} { 
    yylval.ast = new_node("ASSIGN", NULL, yylineno);
    return ASSIGN;
}
{LT} { 
    yylval.ast = new_node("LT", "<", yylineno);
    return LT;
}
{LE} {
    yylval.ast = new_node("LE", "<=", yylineno);
    return LE;
}
{GT} { 
    yylval.ast = new_node("GT", ">", yylineno);
    return GT;    
}
{GE} { 
    yylval.ast = new_node("GE", ">=", yylineno);
    return GE;
}
{NE} { 
    yylval.ast = new_node("NE", "!=", yylineno);
    return NE;
}
{EQ} { 
    yylval.ast = new_node("EQ", "==", yylineno);
    return EQ;
}
{PLUS} { 
    yylval.ast = new_node("PLUS", NULL, yylineno);
    return PLUS;
}
{MINUS} { 
    yylval.ast = new_node("MINUS", NULL, yylineno);
    return MINUS;    
}
{MUL} { 
    yylval.ast = new_node("MUL", NULL, yylineno);
    return MUL;    
}
{DIV} { 
    yylval.ast = new_node("DIV", NULL, yylineno);
    return DIV;
}
{AND} { 
    yylval.ast = new_node("AND", NULL, yylineno);
    return AND;
}
{OR} { 
    yylval.ast = new_node("OR", NULL, yylineno);
    return OR;
}
{NOT} { 
    yylval.ast = new_node("NOT", NULL, yylineno);
    return NOT;
}
{LP} { 
    yylval.ast = new_node("LP", NULL, yylineno);
    return LP;
}
{RP} { 
    yylval.ast = new_node("RP", NULL, yylineno);
    return RP;
}
{LB} { 
    yylval.ast = new_node("LB", NULL, yylineno);
    return LB;
}
{RB} { 
    yylval.ast = new_node("RB", NULL, yylineno);
    return RB;
}
{LC} { 
    yylval.ast = new_node("LC", NULL, yylineno);
    return LC;
}
{RC} { 
    yylval.ast = new_node("RC", NULL, yylineno);
    return RC;
}
[ \r\t] {}
[\n] {has_lexical_error = 0;}
"//" { char c; while((c=input()) != '\n'); }
"/*" {
    char c; 
    while(1) {
        if((c=input()) == '*') {
            if((c=input()) == '/') {
                break;
            }
            else {unput(c);}
        }
        if((c=input()) == '/') {
            if((c=input()) == '*') {
                printf ("Error type B at Line %d: Nested multi-line comments", yylineno);   
                has_error = 1;
            }    
            else {unput(c);}
        }
    }
}
\"[^\"]*\" {
    if(yytext[yyleng-2] == '\\') {
        yyless(yyleng-1);
        yymore();
    } else {
        printf("String detected at line %d, but I do not want to process it!", yylineno);
        has_error = 1;
        has_lexical_error = 1;
    }
}
"*/" {
    printf("Error type B at Line %d: Illegal \"%s\"\n", yylineno, yytext);    
    has_error = 1;
}
. {
    printf("Error type A at Line %d: Mysterious characters \'%s\'\n", 
    yylineno, yytext);
    has_error = 1;
    has_lexical_error = 1;
}
