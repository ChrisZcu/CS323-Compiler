%{
    #include <string.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include"syntax.tab.h"
    #ifndef JSON_C
    #define JSON_C
    #include "json.c"
    #endif
%}
%option noyywrap

unic u[0-9a-fA-F]{4}
esc \\([\"\\/bfnrt]|{unic})
scp [^"\\\x00-\x1f]
string \"({esc}|{scp})*\"

int 0|[1-9][0-9]*
frac \.[0-9]+
exp [Ee][+-]?[0-9]+
number -?{int}{frac}?{exp}?
wrong_number -?0{int}{frac}?{exp}?
empty [ \n\r\t]

%%

"{" { return LC; }
"}" { return RC; }
"[" { return LB; }
"]" { return RB; }
":" { return COLON; }
"," { return COMMA; }

"true" { yylval.JsonObject = new_json_bool(1); return TRUE; }
"false" { yylval.JsonObject = new_json_bool(0); return FALSE; }
"null" { return VNULL; }

{string} { yylval.JsonObject = new_json_string(strdup(yytext)); return STRING; }
{number} { yylval.JsonObject = new_json_number(atof(strdup(yytext))); return NUMBER; }

{empty} {}

{wrong_number} { return WRONGNUMBER; }

. { printf("lexical error: %s\n", yytext); }
