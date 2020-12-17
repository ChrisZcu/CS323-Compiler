%{
    #include <string.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include "syntax.tab.h"
    #ifndef AST_C
    #define AST_C
    #include "ast.cpp"
    #endif

    extern "C" int yylex();

    int yycolumn = 1;
    extern int has_error;
    extern int has_lexical_error;
    #define YY_USER_ACTION yylloc.first_line = yylloc.last_line = yylineno; \
        yylloc.first_column = yycolumn; yylloc.last_column = yycolumn+yyleng-1; \
        yycolumn += yyleng;
%}
%option yylineno 
OCT_INT [-+]?0([0-3]?[0-7]{0,10})
HEX_INT [-+]?0[xX]([0-9a-fA-F]{0,8})
DEC_INT [-+]?0|([1-9][0-9]*)
OCT_INT_WRONG [-+]?0[a-wy-z][0-9a-zA-Z]*
HEX_INT_WRONG [-+]?0[xX][0-9a-zA-Z]*
INT {HEX_INT}|{OCT_INT}|{DEC_INT}
INT_WRONG {HEX_INT_WRONG}|{OCT_INT_WRONG}
OCT_FLOAT {OCT_INT}\.[0-7]*
HEX_FLOAT {HEX_INT}\.[0-9a-f]*
DEC_FLOAT {DEC_INT}\.[0-9]*(e[-+]?[0-9]*)?
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
{INT} {
    yylval.ast = new_node("INT",  (yytext), yylineno);
    return INT;
}
{INT_WRONG} {
    printf("Error type A at Line %d: Wrong Int Form \'%s\'\n", 
    yylineno, yytext);
    has_lexical_error = 1;
    has_error = 1;
}
{FLOAT} { 
    yylval.ast = new_node("FLOAT",  (yytext), yylineno);
    return FLOAT;
}
{CHAR} {
    yylval.ast = new_node("CHAR",  (yytext), yylineno);
    return CHAR;    
}
{CHAR_WRONG} {
    printf("Error type A at Line %d: Wrong Char Form \'%s\'\n", 
    yylineno, yytext);
    has_lexical_error = 1;
    has_error = 1;
}

"int"|"float"|"char" { yylval.ast =new_node("TYPE", yytext, yylineno); return TYPE; }

"struct"     { yylval.ast = new_node("STRUCT",  "", yylineno);return STRUCT; }
"if"         {yylval.ast = new_node("IF",  "", yylineno);return IF;}
"else"       { yylval.ast = new_node("ELSE",  "", yylineno);return ELSE;}
"while"      { yylval.ast = new_node("WHILE",  "", yylineno);return WHILE; }
"return"     { yylval.ast = new_node("RETURN",  "", yylineno);return RETURN; }
"for"        {yylval.ast = new_node("FOR",  "", yylineno);return RETURN; }


{ID} {
    yylval.ast = new_node("ID",  (yytext), yylineno);
    return ID;   
}
{ID_WRONG} {
    printf("Error type A at Line %d: Wrong Form \"%s\"\n", 
    yylineno, yytext);
    has_lexical_error = 1;
    has_error = 1;
}
{DOT} {
    yylval.ast = new_node("DOT",  "", yylineno);
    return DOT;
}
{SEMI} { 
    yylval.ast = new_node("SEMI",  "", yylineno);
    return SEMI;    
}
{COMMA} { 
    yylval.ast = new_node("COMMA",  "", yylineno);
    return COMMA;
}
{ASSIGN} { 
    yylval.ast = new_node("ASSIGN",  "", yylineno);
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
    yylval.ast = new_node("PLUS",  "", yylineno);
    return PLUS;
}
{MINUS} { 
    yylval.ast = new_node("MINUS",  "", yylineno);
    return MINUS;    
}
{MUL} { 
    yylval.ast = new_node("MUL",  "", yylineno);
    return MUL;    
}
{DIV} { 
    yylval.ast = new_node("DIV",  "", yylineno);
    return DIV;
}
{AND} { 
    yylval.ast = new_node("AND",  "", yylineno);
    return AND;
}
{OR} { 
    yylval.ast = new_node("OR",  "", yylineno);
    return OR;
}
{NOT} { 
    yylval.ast = new_node("NOT",  "", yylineno);
    return NOT;
}
{LP} { 
    yylval.ast = new_node("LP",  "", yylineno);
    return LP;
}
{RP} { 
    yylval.ast = new_node("RP",  "", yylineno);
    return RP;
}
{LB} { 
    yylval.ast = new_node("LB",  "", yylineno);
    return LB;
}
{RB} { 
    yylval.ast = new_node("RB",  "", yylineno);
    return RB;
}
{LC} { 
    yylval.ast = new_node("LC",  "", yylineno);
    return LC;
}
{RC} { 
    yylval.ast = new_node("RC",  "", yylineno);
    return RC;
}
[ \r\t] {}
[\n] {has_lexical_error = 0;}
"//" { char c; while((c=yyinput()) != '\n'); }
"/*" {
    char c; 
    while(1) {
        if((c=yyinput()) == '*') {
            if((c=yyinput()) == '/') {
                break;
            } else {unput(c);}
        }
        if((c=yyinput()) == '/') {
            if((c=yyinput()) == '*') {
                printf ("Error type B at Line %d: Nested multi-line comments", yylineno);   
                has_error = 1;
            }    
            else {unput(c);}
        }
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
