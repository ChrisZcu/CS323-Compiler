#include <iostream>
#include <stdlib.h>

#ifndef IRH
#define IRH
#include "ir.hpp"
#endif

using namespace std;

vector<code_node> code;
int val_count = 1;
int label_count = 1;

void new_label(string &name)
{
    string label_num = to_string(label_count);
    label_count++;
    name = "label" + label_num;
}

void new_temp(string &name)
{
    string temp_num = to_string(val_count);
    val_count++;
    name = "t" + temp_num;
}

void add_code(int args_count, ...);

code_node translate_exp(struct ast *exp, string place)
{
    // print_code();
    code_node res;
    if (exp->next_layer->name == "INT")
    {
        res.code_list.emplace_back(place);
        res.code_list.emplace_back(":=");
        res.code_list.emplace_back(exp->next_layer->value);
    }
    else if (exp->next_layer->name == "ID")
    {
        val_d *val = val_find(exp->next_layer->value);
        res.code_list.emplace_back(place);
        res.code_list.emplace_back(":=");
        res.code_list.emplace_back(val->ir_name);
    }
    else if (exp->next_layer->next_neighbor->name == "ASSIGN")
    {
        //TODO, add function get name
    }
    else if (exp->next_layer->next_neighbor->name == "PLUS" ||
             exp->next_layer->next_neighbor->name == "MINUS" ||
             exp->next_layer->next_neighbor->name == "MUL" ||
             exp->next_layer->next_neighbor->name == "DIV")
    {
        string _t1, _t2;
        new_temp(_t1);
        new_temp(_t2);

        code_node _code_1 = translate_exp(exp->next_layer, _t1);

        code_node _code_2 = translate_exp(exp->next_layer->next_neighbor->next_neighbor, _t2);
        code.emplace_back(_code_1);
        code.emplace_back(_code_2);
        string op;
        if (exp->next_layer->next_neighbor->name == "PLUS")
        {
            op = "+";
        }
        else if (exp->next_layer->next_neighbor->name == "MINUS")
        {
            op = "-";
        }
        else if (exp->next_layer->next_neighbor->name == "MUL")
        {
            op = "*";
        }
        else if (exp->next_layer->next_neighbor->name == "DIV")
        {
            op = "/";
        }
        res.code_list.emplace_back(place);
        res.code_list.emplace_back(":=");
        res.code_list.emplace_back(_t1);
        res.code_list.emplace_back(op);
        res.code_list.emplace_back(_t2);
    }
    else if (exp->next_layer->name == "MINUS")
    {
        //TODO
        string _t1;
        new_temp(_t1);
        code_node _code_1 = translate_exp(exp->next_layer->next_neighbor, _t1);
        code.emplace_back(_code_1);
        res.code_list.emplace_back(place);
        res.code_list.emplace_back(":=");
        res.code_list.emplace_back("#0");
        res.code_list.emplace_back("-");
        res.code_list.emplace_back(_t1);
    }
    else if (exp->next_layer->next_neighbor->name == "LT" ||
             exp->next_layer->next_neighbor->name == "LE" ||
             exp->next_layer->next_neighbor->name == "GT" ||
             exp->next_layer->next_neighbor->name == "GE" ||
             exp->next_layer->next_neighbor->name == "NE" ||
             exp->next_layer->next_neighbor->name == "EQ" ||
             exp->next_layer->next_neighbor->name == "AND" ||
             exp->next_layer->next_neighbor->name == "OR" ||
             exp->next_layer->name == "NOT")

    {
        //TODO condition
    }
    else if (exp->next_layer->name == "ID")
    { //function
        if (exp->next_layer->next_neighbor->next_neighbor->name == "RP")
        {

            if (exp->next_layer->value == "read")
            {
                res.code_list.emplace_back("READ");
                res.code_list.emplace_back(place);
            }
            else
            {
                res.code_list.emplace_back(place);
                res.code_list.emplace_back(":=");
                res.code_list.emplace_back("CALL");
                res.code_list.emplace_back(exp->next_layer->value);
            }
        }else{
            if (exp->next_layer->value == "write")
            {
                
            }else{
                
            }
            
        }
    }

    return res;
}

code_node translate_stmt(struct ast *stmt);
code_node translate_cond(struct ast *exp, string label_true, string label_false);
code_node translate_args(struct ast *args);

void function_init(struct ast *fun_dec)
{
    code_node func_init_node;
    func_init_node.code_list.emplace_back("FUNCTION");
    string name = fun_dec->next_layer->value;
    func_init_node.code_list.emplace_back(name);
    func_init_node.code_list.emplace_back(":");
    code.emplace_back(func_init_node);
    if (fun_dec->next_layer->next_neighbor->next_neighbor->name == "VarList")
    { //param
        func_d *func = func_find(name);
        for (val_d p : func->args)
        {
            string _t;
            new_temp(_t);
            p.ir_name = _t;
            // cout<<p.name<<", " + p.ir_name<<endl;
            val_d *_tmp = val_find(p.name);
            _tmp->ir_name = _t;
            code_node _code;

            _code.code_list.emplace_back("PARAM");
            _code.code_list.emplace_back(_t);
            code.emplace_back(_code);
        }
    }
};
void function_def_list(struct ast *def_list)
{
    struct ast *def = def_list->next_layer;

    while (def != NULL)
    {
        struct ast *dec = def->next_layer->next_neighbor->next_layer;
        while (dec != NULL)
        { //int a,b,c,d=10;
            struct ast *var_dec = dec->next_layer;
            if (var_dec->next_neighbor == NULL)
            { //int a
                string name;
                new_temp(name);
                code_node _code;
                _code.code_list.emplace_back("DEC");
                _code.code_list.emplace_back(name);
                _code.code_list.emplace_back(to_string(8));
                code.emplace_back(_code);
                string _name = get_var_dec_name(var_dec);
                val_d *val = val_find(_name);
                val->ir_name = name;
            }
            else
            { //int a=0
                string name;
                new_temp(name);
                code_node exp_node = translate_exp(var_dec->next_neighbor->next_neighbor, name);
                code.emplace_back(exp_node);
                string _name = get_var_dec_name(var_dec);
                val_d *val = val_find(_name);
                val->ir_name = name;
            }
            if (dec->next_neighbor != NULL)
            {
                dec = dec->next_neighbor->next_neighbor;
            }
            else
            {
                break;
            }
        }
        def = def->next_neighbor->next_layer;
    }
}
void function_stmt_list(struct ast *stmt_list){};

void function_handle(struct ast *fun_dec)
{
    function_init(fun_dec);
    struct ast *comp_st = fun_dec->next_neighbor;

    struct ast *def_list = comp_st->next_layer->next_neighbor; // int a;int a=1;
    function_def_list(def_list);

    struct ast *stmt_list = def_list->next_neighbor;
    function_stmt_list(stmt_list);
}

void ir_translate(struct ast *ast)
{
    if (ast != NULL)
    {
        if (ast->name.size() > 0)
        {
            if (ast->name == "ExtDef")
            { //struct or function
                if (ast->next_layer->next_neighbor->name == "FunDec")
                { //function
                    function_handle(ast->next_layer->next_neighbor);
                }
                else if (ast->next_layer->next_neighbor->name == "SEMI")
                { //struct, no global variable
                }
            }
        }
        struct ast *son = ast->next_layer;
        while (son != NULL)
        {
            ir_translate(son);
            son = son->next_neighbor;
        }
    }
}

// int main()
// {
//     string name;
//     new_label(name);
//     cout << name << endl;
//     return 0;
// }
