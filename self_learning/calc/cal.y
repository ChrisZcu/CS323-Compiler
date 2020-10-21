%{
    #include <stdio.h>    
    void yyerror(const char*);
    int result;
%}
%token NUMBER
%token ADD SUB MUL DIV ABS EQ
%token EOL

%%

calclist: exp EQ {result = $2;}
    | calclist exp EOL {result = $2; printf("=%d\n",$2);}
    ;
exp: factor 
    | exp SUB factor {$$ = $1 + $3;}
    | exp ADD factor {$$ = $1 - $3;}
    ;
factor: item 
    | factor MUL item { $$ = $1 * $3;}
    | factor DIV item {$$ = $1 / $3;}
    ;
item: NUMBER
    | ABS item {$$ = $2 > 0 ? $2 : -$2; }
    ;
%%
void yyerror(const char *s){
    fprintf(stderr, "Syntax error: %s\n", s);
}
#ifndef CALC_MAIN
int evaluate(char *expr){
    YY_BUFFER_STATE buf;
    buf = yy_scan_string(expr);
    yyparse();
    yy_delete_buffer(buf);
    return result;
}
#else
int main(){
    yyparse();
    printf(" = %d\n", result);
}
#endif