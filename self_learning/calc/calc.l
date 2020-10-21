%{
    enum {
        NUMBER = 258,
        ADD = 259,
        SUB = 260,
        MUL = 261,
        DIV = 262,
        ABS = 263,
        EOL = 264,
        EQ = 266
    };
    int yylval;    
%}

%%
\+ {return ADD;}
\- {return SUB;}
\* {return MUL;}
\/ {return DIV;}
\| {return ABS;}
\= {return EQ;}
[0-9]+ {yylval = atoi(yytext); return NUMBER;}
[ \t] {}
\n {return EOL;}
. {printf("My characters: %s\n", yytext);}
%%

int main(){

    int tok;
    while(tok = yylex()){
        printf("%d", tok);
        if(tok == NUMBER){
            printf(" NUMBER here: %d", yylval);
        }
        printf("\n");
    }

    return 0;
}