
#include <string>
#include <iostream>
#ifndef STDARG_H
#define STDARG_H
#include <stdarg.h>
#endif

#ifndef SYMTABLE
#define SYMTABLE
#include "symtable.cpp"
#endif

#ifndef IR
#define IR
#include "ir.cpp"
#endif

using namespace std;

struct ast *new_ast(string name, int num, ...)
{

    va_list valist;
    struct ast *node = (struct ast *)malloc(sizeof(struct ast));
    struct ast *tmp = (struct ast *)malloc(sizeof(struct ast));
    if (!name.empty())
    {
        node->name = name;
    }

    va_start(valist, num);

    if (num > 0)
    {
        tmp = va_arg(valist, struct ast *);
        node->next_layer = tmp;
        node->lineno = tmp->lineno;
        num--;
        while (num > 0)
        {
            tmp->next_neighbor = va_arg(valist, struct ast *);
            tmp = tmp->next_neighbor;
            num--;
        }
    }

    return node;
}

struct ast *new_node(string name, string value, int lineno)
{
    struct ast *node = (struct ast *)malloc(sizeof(struct ast));
    memset(node, '\0', sizeof(ast));

    node->name = name;

    if (!value.empty())
    {
        node->value = value;
    }
    node->lineno = lineno;

    return node;
}

void parsetree(struct ast *ast, int level)
{
    if (ast != NULL)
    {
        if (ast->name.size() > 0)
        {
            for (int i = 0; i < level; i++)
            {
                cout << " ";
            }
            if (ast->lineno != -1)
            {
                cout << ast->name;
            }
            if (ast->name == "TYPE" ||
                (ast->name == "ID") ||
                (ast->name == "INT") ||
                (ast->name == "FLOAT") ||
                (ast->name == "CHAR"))
            {
                cout << ": " + ast->value;
            }
            if ((ast->name == "Program") ||
                (ast->name == "Specifier") ||
                (ast->name == "ExtDef") ||
                (ast->name == "Dec") ||
                (ast->name == "StmtList") ||
                (ast->name == "ExtDecList") ||
                (ast->name == "StructSpecifier") ||
                (ast->name == "VarDec") ||
                (ast->name == "FunDec") ||
                (ast->name == "Exp") ||
                (ast->name == "VarList") ||
                (ast->name == "ParamDec") ||
                (ast->name == "CompSt") ||
                (ast->name == "ExtDefList") ||
                (ast->name == "Stmt") ||
                (ast->name == "DefList") ||
                (ast->name == "Def") ||
                (ast->name == "DecList") ||
                (ast->name == "Args"))
            {
                cout << " (" << ast->lineno << ")";
            }
            cout << endl;
        }
        struct ast *son = ast->next_layer;
        while (son != NULL)
        {
            parsetree(son, level + 1);
            son = son->next_neighbor;
        }
    }
}

void func_args(struct ast *var_list, vector<val_d> &args)
{
    struct ast *_tmp_head = var_list;
    while (_tmp_head != NULL)
    {
        struct ast *param_dec = _tmp_head->next_layer;
        string type = get_specifier_type(param_dec->next_layer);
        struct ast *var_dec = param_dec->next_layer->next_neighbor;

        val_d val = get_val(type, var_dec);
        val.is_func_args = true;
        args.emplace_back(val);
        symbol.val.emplace_back(val);
        if (param_dec->next_neighbor != NULL)
        {
            _tmp_head = param_dec->next_neighbor->next_neighbor;
        }
        else
        {
            break;
        }
    }
}

void func_struct_insert(struct ast *ast)
{
    struct ast *specifier = ast->next_layer;

    if (specifier->next_neighbor->name == "FunDec") //function
    {
        func_d func;
        func.return_type = specifier->next_layer->value;
        if (specifier->next_neighbor->next_layer->next_neighbor->next_neighbor->name != "RP") //args
        {
            struct ast *args = specifier->next_neighbor->next_layer->next_neighbor->next_neighbor;
            func_args(args, func.args);
        }
        func.name = specifier->next_neighbor->next_layer->value;
        symbol.func.emplace_back(func);
    }
    else if (specifier->next_neighbor->name == "SEMI")
    { //struct
        struct ast *struct_layer = specifier->next_layer->next_layer;
        if (struct_layer->next_neighbor->next_neighbor != NULL)
        { //define struct
            string struct_name = struct_layer->next_neighbor->value;
            val_d val;
            val.type = struct_name;
            val.name = struct_name;
            //TODO field handle
            struct ast *def_list = struct_layer->next_neighbor->next_neighbor->next_neighbor;
            struct_variable_insert(def_list, val.struct_field);
            symbol.val.emplace_back(val);
        }
    }
    else
    { //global variable
    }
}

string get_specifier_type(struct ast *specifier)
{
    return (specifier->next_layer->name == "TYPE") ? specifier->next_layer->value
                                                   : specifier->next_layer->next_layer->next_neighbor->value;
}
bool is_struct = false;
void struct_variable_insert(struct ast *ast, vector<val_d> &val_vec)
{
    is_struct = true;
    if (ast->next_layer == NULL)
    { //struct for first line, hanle it before
        return;
    }
    struct ast *def = ast->next_layer;
    struct ast *specifier = def->next_layer;

    if (specifier->next_layer->name == "TYPE")
    { //variable
        string type = specifier->next_layer->value;
        dec_list_charge(type, specifier->next_neighbor, val_vec);
    }
    else
    { //struct
        string type = specifier->next_layer->next_layer->next_neighbor->value;
        dec_list_charge(type, specifier->next_neighbor, val_vec);
    }

    if (def->next_neighbor != NULL)
    {
        struct_variable_insert(def->next_neighbor, val_vec);
    }
    // ast->next_layer = NULL;
}

void local_variable_insert(struct ast *ast, vector<val_d> &val_vec)
{
    if (ast->next_layer == NULL)
    { //struct for first line, hanle it before
        return;
    }
    struct ast *def = ast->next_layer;
    struct ast *specifier = def->next_layer;

    if (specifier->next_layer->name == "TYPE")
    { //variable
        string type = specifier->next_layer->value;
        dec_list_charge(type, specifier->next_neighbor, val_vec);
    }
    else
    { //struct
        string type = specifier->next_layer->next_layer->next_neighbor->value;
        dec_list_charge(type, specifier->next_neighbor, val_vec);
    }

    // if (def->next_neighbor != NULL)
    // {
    //     local_variable_insert(def->next_neighbor, val_vec);
    // }
    // ast->next_layer = NULL;
}

void dec_list_charge(string type, struct ast *ast, vector<val_d> &val_vec)
{
    struct ast *dec = ast->next_layer;
    dec_charge(type, dec, val_vec);
    if (dec->next_neighbor != NULL)
    {
        ast = dec->next_neighbor->next_neighbor;
        dec_list_charge(type, ast, val_vec);
    }
}

void dec_charge(string type, struct ast *ast, vector<val_d> &val_vec)
{
    int i;
    i = var_dec_charge(type, ast->next_layer, val_vec);
    if (i >= 0 && ast->next_layer->next_neighbor != NULL)
    { //assign
    }
}

string get_var_dec_name(struct ast *ast)
{
    string name;
    if (ast->next_layer->name == "ID")
    {
        name = ast->next_layer->value;
    }
    else
    { //array
        struct ast *tmp_var_dec = ast;
        while ((tmp_var_dec->next_layer->name == "VarDec"))
        {
            tmp_var_dec = tmp_var_dec->next_layer;
        }
        name = tmp_var_dec->next_layer->value;
    }
    return name;
}

val_d get_val(string type, struct ast *ast)
{
    int i;
    i = 0;
    vector<int> dim_num;
    string name;
    if (ast->next_layer->name == "ID")
    {
        name = ast->next_layer->value;
    }
    else
    { //array
        struct ast *tmp_var_dec = ast;
        while ((tmp_var_dec->next_layer->name == "VarDec"))
        {
            i++;
            string dim = tmp_var_dec->next_layer->next_neighbor->next_neighbor->value;
            dim_num.emplace_back(stoi(dim));
            tmp_var_dec = tmp_var_dec->next_layer;
        }
        name = tmp_var_dec->next_layer->value;
    }

    val_d val;
    val.type = type;
    val.dim = i;
    for (int i = dim_num.size() - 1; i > -1; i--)
    {
        val.dim_num.emplace_back(dim_num[i]);
    }

    val.name = name;
    return val;
}

int var_dec_charge(string type, struct ast *ast, vector<val_d> &val_vec)
{
    val_d val = get_val(type, ast);
    val_vec.emplace_back(val);
}

void gen_symbol_table(struct ast *ast, int level)
{
    if (ast != NULL)
    {
        if (ast->name.size() > 0)
        {

            if (ast->lineno != -1)
            {
                if (ast->name == "ExtDef") //function, struct, global variable
                {
                    func_struct_insert(ast);
                }
                else if (ast->name == "DefList") //local variable
                {
                    local_variable_insert(ast, symbol.val);
                }
                else if (ast->name == "Exp")
                {
                    exp_cut(ast);
                }
            }
        }

        struct ast *son = ast->next_layer;
        // if (is_struct)
        // {
        //     son = NULL;
        //     is_struct = false;
        // }

        while (son != NULL)
        {
            gen_symbol_table(son, level + 1);
            son = son->next_neighbor;
        }
    }
}

void print_symbol()
{
    for (int i = 0; i < symbol.func.size(); i++)
    {
        func_d func = symbol.func[i];
        cout << func.name + "(" << func.args.size() << "), ";
    }
    cout << endl;
    for (int i = 0; i < symbol.val.size(); i++)
    {
        val_d val = symbol.val[i];
        cout << val.name << "," << val.type << "," << val.dim << "; ";
    }
    cout << endl;
}

void print_code()
{
    for (int i = 0; i < code.size(); i++)
    {
        if (code[i].code_list.size() == 0)
        {
            continue;
        }

        for (string e : code[i].code_list)
        {
            cout << e << " ";
        }
        cout << endl;
    }
}

bool exp_cut(struct ast *exp)
{

    if (exp->next_layer->name == "Exp")
    {
        if (exp->next_layer->next_neighbor->name == "PLUS" ||
            exp->next_layer->next_neighbor->name == "MINUS" ||
            exp->next_layer->next_neighbor->name == "MUL" ||
            exp->next_layer->next_neighbor->name == "DIV")
        {
            struct ast *exp1 = exp->next_layer;
            struct ast *exp2 = exp1->next_neighbor->next_neighbor;
            bool if_num_1 = exp_cut(exp1);

            // if (exp2->next_layer->name == "ID")
            // {
            //     cout << exp1->next_layer->value << endl;
            //     parsetree(exp, 0);
            //     cout<<endl;
            // }
            bool if_num_2 = exp_cut(exp2);
            if (if_num_1 && if_num_2)
            {
                string value;
                if (exp1->next_neighbor->name == "PLUS")
                {
                    value = to_string(stoi(exp1->next_layer->value) + stoi(exp2->next_layer->value));
                }
                else if (exp1->next_neighbor->name == "MINUS")
                {

                    value = to_string(stoi(exp1->next_layer->value) - stoi(exp2->next_layer->value));
                }
                else if (exp1->next_neighbor->name == "MUL")
                {

                    value = to_string(stoi(exp1->next_layer->value) * stoi(exp2->next_layer->value));
                }
                else
                {
                    value = to_string(stoi(exp1->next_layer->value) / stoi(exp2->next_layer->value));
                }
                string name = "INT";
                struct ast *exp_tmp = new_node(name, value, exp->lineno);
                exp->next_layer = exp_tmp;
                // cout << exp->next_layer->value << ", " << exp->next_neighbor->next_neighbor->next_layer->value << endl;
                return true;
            }
            else
            {
                if (if_num_1) //prefix
                {
                    if (exp->next_layer->next_layer->value == "0" && exp->next_layer->next_neighbor->name == "PLUS" || //0+
                        exp->next_layer->next_layer->value == "1" && exp->next_layer->next_neighbor->name == "MUL"     //1*
                    )
                    {
                        /* code */
                        exp->next_layer = exp->next_layer->next_neighbor->next_neighbor->next_layer;
                    }
                }
                else if (if_num_2)
                {
                    if (exp->next_layer->next_neighbor->next_neighbor->next_layer->value == "0" && exp->next_layer->next_neighbor->name == "PLUS" || //+/-0
                        exp->next_layer->next_neighbor->next_neighbor->next_layer->value == "0" && exp->next_layer->next_neighbor->name == "MINUS" ||
                        exp->next_layer->next_neighbor->next_neighbor->next_layer->value == "1" && exp->next_layer->next_neighbor->name == "MUL" ||
                        exp->next_layer->next_neighbor->next_neighbor->next_layer->value == "1" && exp->next_layer->next_neighbor->name == "DIV")
                    {
                        /* code */
                        exp->next_layer = exp->next_layer->next_layer;
                    }
                }

                return false;
            }
        }
    }
    else if (exp->next_layer->name == "INT" ||
             exp->next_layer->name == "FLOAT" ||
             exp->next_layer->name == "CHAR")
    {
        return true;
    }
    // parsetree(exp, 0);
    // cout << endl;

    return false;
}

void semantic(struct ast *ast, int level)
{
    symbal_init();
    gen_symbol_table(ast, level);
    ir_translate(ast);
    print_code();
    // print_symbol();
    // parsetree(ast, level);
}
