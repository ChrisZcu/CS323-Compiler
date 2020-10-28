%{
    #ifndef LEX_YY_C
    #define LEX_YY_C
    #include "lex.yy.c"
    #endif

    #ifndef AST_C
    #define AST_C
    #include "ast.c"
    #endif
    extern FILE *yyin;
    void yyerror(const char*);
    int has_error = 0;
    int has_lexical_error = 0;
    

%}
%union {
    struct ast *ast;
}
%error-verbose
%nonassoc LOWER_ELSE
%nonassoc <ast> TYPE STRUCT IF ELSE WHILE RETURN
%token <ast> INT FLOAT CHAR ID
%token <ast> LC RC
%right <ast> ASSIGN
%left <ast> AND 
%left <ast> OR 
%left <ast> LT LE GT GE NE EQ
%left <ast> PLUS MINUS
%left <ast> MUL DIV
%right <ast> NOT
%left <ast> LP RP LB RB DOT
%token <ast> SEMI COMMA

%type <ast> Program ExtDefList ExtDef ExtDecList Specifier StructSpecifier VarDec FunDec VarList ParamDec CompSt StmtList Stmt DefList Def DecList Dec Exp Args
%%
Program: ExtDefList {
        $$ = new_ast("Program", 1, $1);
        int i = @1.first_line; 
        if (!has_error) {
            semantic($$, 0);
        }
    }
    ;
ExtDefList: ExtDef ExtDefList {
        $$ = new_ast("ExtDefList", 2, $1, $2);
    }
    | %empty {
        $$ = new_ast(NULL, 0);
    }
    ;
ExtDef: Specifier ExtDecList SEMI {
        $$ = new_ast("ExtDef", 3, $1, $2, $3);
    }
    | Specifier SEMI {
        $$ = new_ast("ExtDef", 2, $1, $2);
    }
    | Specifier FunDec CompSt {
        $$ = new_ast("ExtDef", 3, $1, $2, $3);
    }
    ;
ExtDecList: VarDec {
        $$ = new_ast("ExtDecList", 1, $1);
    }
    | VarDec COMMA ExtDecList {
        $$ = new_ast("ExtDecList", 3, $1, $2, $3);
    }
    ;

Specifier: TYPE {
         $$ = new_ast("Specifier", 1, $1);
    } 
    | StructSpecifier {
         $$ = new_ast("Specifier", 1, $1);
    }
    ;

StructSpecifier: STRUCT ID LC DefList RC {
        $$ = new_ast("StructSpecifier", 5, $1, $2, $3, $4, $5);
    }
    |
    STRUCT ID {
        $$ = new_ast("StructSpecifier", 2, $1, $2);
    }
    ;

VarDec: ID {
        $$ = new_ast("VarDec", 1, $1);
    }
    | VarDec LB INT RB {
        $$ = new_ast("VarDec", 4, $1, $2, $3, $4);
    }
    ;

FunDec: ID LP VarList RP {
        $$ = new_ast("FunDec", 4, $1, $2, $3, $4);
    }
    | ID LP RP {
        $$ = new_ast("FunDec", 3, $1, $2, $3);
    }
    ;
VarList: ParamDec COMMA VarList {
       $$ = new_ast("VarList", 3, $1, $2, $3);
    }
    | ParamDec {
       $$ = new_ast("VarList", 1, $1);
    }
    ;
ParamDec: Specifier VarDec {
        $$ = new_ast("ParamDec", 2, $1, $2);
    }
    ;

CompSt: LC DefList StmtList RC {
        $$ = new_ast("CompSt", 4, $1, $2, $3, $4);
    }
    ;
StmtList: Stmt StmtList {
        $$ = new_ast("StmtList", 2, $1, $2);
    } 
    | %empty {
        $$ = new_ast(NULL, 0);
    }
    ;
Stmt: Exp SEMI {
        $$ = new_ast("Stmt", 2, $1, $2);
    }
    | CompSt {
        $$ = new_ast("Stmt", 1, $1);
    }
    | RETURN Exp SEMI {
        $$ = new_ast("Stmt", 3, $1, $2, $3);
    }
    | IF LP Exp RP Stmt %prec LOWER_ELSE {
        $$ = new_ast("Stmt", 5, $1, $2, $3, $4, $5);
    }
    | IF LP Exp RP Stmt ELSE Stmt {
        $$ = new_ast("Stmt", 7, $1, $2, $3, $4, $5, $6, $7);
    }
    | WHILE LP Exp RP Stmt {
        $$ = new_ast("Stmt", 5, $1, $2, $3, $4, $5);
    }
    ;

DefList: Def DefList {//TODO variable defination
       $$ = new_ast("DefList", 2, $1, $2);
    }
    | %empty {
       $$ = new_ast(NULL, 0);
    }
    ;

Def: Specifier DecList SEMI {
        $$ = new_ast("Def", 3, $1, $2, $3);
    }
    ;

DecList: Dec {
        $$ = new_ast("DecList", 1, $1);
    } 
    | Dec COMMA DecList {
        $$ = new_ast("DecList", 3, $1, $2, $3);
    }
    ;

Dec: VarDec {
        $$ = new_ast("Dec", 1, $1);
    }
    | VarDec ASSIGN Exp {
        $$ = new_ast("Dec", 3, $1, $2, $3);
    }
    ;

Exp: Exp ASSIGN Exp {$$ = new_ast("Exp", 3, $1, $2, $3);}
    | Exp AND Exp {$$ = new_ast("Exp", 3, $1, $2, $3);}
    | Exp OR Exp {$$ = new_ast("Exp", 3, $1, $2, $3);}
    | Exp LT Exp {$$ = new_ast("Exp", 3, $1, $2, $3);}
    | Exp LE Exp {$$ = new_ast("Exp", 3, $1, $2, $3);}
    | Exp GT Exp {$$ = new_ast("Exp", 3, $1, $2, $3);}
    | Exp GE Exp {$$ = new_ast("Exp", 3, $1, $2, $3);}
    | Exp NE Exp {$$ = new_ast("Exp", 3, $1, $2, $3);}
    | Exp EQ Exp {$$ = new_ast("Exp", 3, $1, $2, $3);}
    | Exp PLUS Exp {$$ = new_ast("Exp", 3, $1, $2, $3);}
    | Exp MINUS Exp {$$ = new_ast("Exp", 3, $1, $2, $3);}
    | Exp MUL Exp {$$ = new_ast("Exp", 3, $1, $2, $3);}
    | Exp DIV Exp {$$ = new_ast("Exp", 3, $1, $2, $3);}
    | LP Exp RP {$$ = new_ast("Exp", 3, $1, $2, $3);}
    | MINUS Exp {$$ = new_ast("Exp", 2, $1, $2);}
    | NOT Exp {$$ = new_ast("Exp", 2, $1, $2);}
    | ID LP Args RP {$$ = new_ast("Exp", 4, $1, $2, $3, $4);}
    | ID LP RP {$$ = new_ast("Exp", 3, $1, $2, $3);}
    | Exp LB Exp RB {$$ = new_ast("Exp", 4, $1, $2, $3, $4);}
    | Exp DOT ID {$$ = new_ast("Exp", 3, $1, $2, $3);}
    | ID {$$ = new_ast("Exp", 1, $1);}
    | INT {$$ = new_ast("Exp", 1, $1);}
    | FLOAT {$$ = new_ast("Exp", 1, $1);}
    | CHAR {$$ = new_ast("Exp", 1, $1);}
    | error {}
    ;

Args: Exp COMMA Args {$$ = new_ast("Args", 3, $1, $2, $3);}
    | Exp {$$ = new_ast("Args", 1, $1);}
    ;


%%
/*
ExtDef: Specifier error SEMI
ExtDef: Specifier error 

Stmt: Exp error
Stmt: RETURN Exp error
Stmt: RETURN error SEMI
Def: Specifier DecList error 
Def: Specifier error SEMI 

Dec: VarDec ASSIGN error

*/
void yyerror(const char *s){
    has_error = 1;
    
    if (!has_lexical_error) {
        fprintf(stderr, "Error type B at line %d: %s\n", yylineno, s);
    }
}

int main(int argc, char **argv){
    yyin = fopen(argv[1], "r");
    yyparse();
    fclose(yyin);
}


