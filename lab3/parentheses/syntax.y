%{
    #include"lex.yy.c"
    void yyerror(const char *s);
    int result;
%}
%token LP RP LB RB LC RC
%%
Quiz: String {result = 1;}
    ;

String: %empty
    | String LP String RP String
    | String LB String RB String
    | String LC String RC String
    ;
%%

void yyerror(const char *s){
    result = 0;
}

int validParentheses(char *expr){
    yy_scan_string(expr);
    yyparse();
    return result;
}
