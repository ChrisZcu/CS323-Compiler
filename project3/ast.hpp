#include<string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

struct ast
{
    string name;
    string value;
    int lineno;

    struct ast *next_layer;
    struct ast *next_neighbor;
};
string get_specifier_type(struct ast *ast);
string get_var_dec_name(struct ast *ast);
void print_code();
bool exp_cut(struct ast *exp);