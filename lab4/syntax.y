%{
    #include"lex.yy.c"
    void yyerror(const char*);
%}

%token LC RC LB RB COLON COMMA
%token STRING NUMBER
%token WRONGNUMBER
%token TRUE FALSE VNULL EMPTY
%%

Json:
      Value
    ;
Value:
    WRONGNUMBER error { puts("Numbers cannot have leading zeroes, recovered"); }
    | Object
    | Array
    | STRING
    | NUMBER
    | TRUE
    | FALSE
    | VNULL
    | Array COMMA error { puts("Comma after the close, recovered"); }
    | Object Values error { puts("misplace quoted value, recovered"); }
    ;
Object:
      LC RC
    | LC Members RC
    | LC Member COMMA error { puts("Comma instead of closing brace, recovered"); }
    ;
Members:
      Member
    | Member COMMA Members
    | Member COMMA RC error { puts("Extra comma, recovered"); }
    ;
Member:
      STRING COLON Value
    | STRING Value error { puts("Missing colon, recovered"); }
    | STRING COMMA Value error { puts("Comma instead of colon, recovered"); }
    | STRING COLON COLON Value error { puts("double colon, recovered"); }
    ;
Array:
      LB RB
    | LB Values RB
    | LB Values RC error { puts("unmatched right bracket, recovered"); }
    | LB Values RB RB error { puts("Extra closed array, recovered"); }
    | LB Values error { puts("Unclosed array, recovered"); }
    ;
Values:
      Value
    | Value COMMA Values
    | Value COMMA error { puts("extra comma, recovered"); }
    | Value COMMA COMMA error { puts("double extra comma, recovered"); }
    | Value COLON Values error { puts("Colon instead of comma, recovered"); }
    | COMMA Values error { puts("missing value, recovered"); }
    ;
%%

void yyerror(const char *s){
    printf("syntax error: ");
}

int main(int argc, char **argv){
    if(argc != 2) {
        fprintf(stderr, "Usage: %s <file_path>\n", argv[0]);
        exit(-1);
    }
    else if(!(yyin = fopen(argv[1], "r"))) {
        perror(argv[1]);
        exit(-1);
    }
    yyparse();
    return 0;
}
