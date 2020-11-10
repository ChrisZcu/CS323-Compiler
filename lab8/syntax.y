%{
    #include"lex.yy.c"
    #ifndef JSON_C
    #define JSON_C
    #include "json.c"
    #endif
    #include <stdio.h>

    void yyerror(const char*);
    struct ArrayValue *array_value;
    struct ArrayValue *array_head;
%}
%union {
    struct JsonObject *JsonObject; // for node
    struct ObjectMember *ObjectMember; // for kv
}


%token <JsonObject>LC RC LB RB COLON COMMA
%token <JsonObject>STRING NUMBER
%token <JsonObject>WRONGNUMBER
%token <JsonObject>TRUE FALSE 
%token <JsonObject> VNULL EMPTY
%type <JsonObject> Value  Array Object Member Members
%type <ObjectMember>  Json

%%

Json:
      Value {$$ = new_object_member_($1);}
    ;
Value:
    WRONGNUMBER error { puts("Numbers cannot have leading zeroes, recovered"); }
    | Object {$$ = $1;}
    | Array {$$ = $1;}
    | STRING {$$ = $1;}
    | NUMBER {$$ = $1;}
    | TRUE {$$ = $1;}
    | FALSE {$$ = $1;}
    | VNULL {$$ = $1;}
    | Array COMMA error { puts("Comma after the close, recovered"); }
    | Object Values error { puts("misplace quoted value, recovered"); }
    ;
Object:
      LC RC
    | LC Members RC {$$ = $2; }
    | LC Member COMMA error { puts("Comma instead of closing brace, recovered"); }
    ;
Members:
      Member {$$ = $1;}
    | Member COMMA Members {$$ =json_add_object($3, $1); }
    | Member COMMA RC error { puts("Extra comma, recovered"); }
    ;
Member:
      STRING COLON Value {struct ObjectMember *tmp_member = new_object_member($1->string, $3, NULL);
                            $$ = new_json_object(tmp_member);
    }
    | STRING Value error { puts("Missing colon, recovered"); }
    | STRING COMMA Value error { puts("Comma instead of colon, recovered"); }
    | STRING COLON COLON Value error { puts("double colon, recovered"); }
    ;
Array:
      LB RB
    | LB Values RB { $$ = new_json_array(array_head); }
    | LB Values RC error { puts("unmatched right bracket, recovered"); }
    | LB Values RB RB error { puts("Extra closed array, recovered"); }
    | LB Values error { puts("Unclosed array, recovered"); }
    ;
Values:
      Value {
            array_value = new_array_value($1);
            array_head = array_value;
      }
    | Value COMMA Values { 
        array_value->next = new_array_value($1);
        array_value = array_value->next;
        }
    | Value COMMA error { puts("extra comma, recovered"); }
    | Value COMMA COMMA error { puts("double extra comma, recovered"); }
    | Value COLON Values error { puts("Colon instead of comma, recovered"); }
    | COMMA Values error { puts("missing value, recovered"); }
    ;
%%

void yyerror(const char *s){
    printf("syntax error: ");
}

