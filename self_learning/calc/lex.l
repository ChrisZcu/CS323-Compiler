%{
        
%}
%%
\+ {printf("PLUS\n");}
\- {printf("MINUS\n");}
\* {printf("MULTIPLE\n");}
\/ {printf("DIVIDE\n");}
\| {printf("ABS\n");}

[0-9]+ {printf("%s\n", yytext);}
[ \t] {}
. {printf("My characters %s", yytext);}
%%