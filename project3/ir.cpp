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

void exp_opt(struct ast *exp, string &t)
{
    if (exp->next_layer->name == "ID" && exp->next_layer->next_neighbor == NULL)
    {
        string name = exp->next_layer->value;
        val_d *val = val_find(name);
        t = val->ir_name;
    }
    else if (exp->next_layer->name == "INT")
    {
        t = "#" + exp->next_layer->value;
    }
    else
    {
        new_temp(t);
        code_node _code_2 = translate_exp(exp, t);
        code.emplace_back(_code_2);
    }
}
code_node translate_exp(struct ast *exp, string place)
{

    if (exp->next_layer->name == "LP")
    {
        return translate_exp(exp->next_layer->next_neighbor, place);
    }

    code_node res;
    if (exp->next_layer->name == "INT")
    {
        res.code_list.emplace_back(place);
        res.code_list.emplace_back(":=");
        res.code_list.emplace_back("#" + exp->next_layer->value);
    }
    else if (exp->next_layer->name == "ID" && exp->next_layer->next_neighbor == NULL)
    {
        val_d *val = val_find(exp->next_layer->value);
        res.code_list.emplace_back(place);
        res.code_list.emplace_back(":=");
        res.code_list.emplace_back(val->ir_name);
    }
    else if (exp->next_layer->next_neighbor->name == "ASSIGN") //局部变量调用才有，传入place为irname
    {
        if (exp->next_layer->next_layer->name == "ID")
        { //a =..
            res = translate_exp(exp->next_layer->next_neighbor->next_neighbor, place);
        }
        else if (exp->next_layer->next_layer->next_neighbor->name == "DOT")
        { //a.s=
        }
        else
        { //array, a[1][2][3] = ...
            string _t1;
            new_temp(_t1);
            code_node _code_1 = translate_exp(exp->next_layer->next_neighbor->next_neighbor, _t1);
            code.emplace_back(_code_1);

            struct ast *tmp_exp = exp->next_layer->next_layer;
            vector<string> ary_dim;
            int dim = 0;
            while (tmp_exp->name != "ID")
            {
                dim++;
                string _t;
                new_temp(_t);
                code.emplace_back(translate_exp(tmp_exp->next_neighbor->next_neighbor, _t));
                ary_dim.emplace_back(_t);
                tmp_exp = tmp_exp->next_layer;
            }

            string ary_name = tmp_exp->value;
            val_d *ary_val = val_find(ary_name);
            tmp_exp = exp->next_layer->next_layer;
            string off = "&" + ary_val->ir_name;
            string final_addr;
            if (ary_dim.size() == ary_val->dim_num.size())
            { //full
                for (int i = 0; i < ary_dim.size() - 1; i++)
                {
                    string index_ir = ary_dim[ary_dim.size() - i - 1];
                    int mul_dim = ary_val->dim_num[i + 1];
                    for (int j = i + 2; j < ary_val->dim; j++)
                    {
                        mul_dim *= ary_val->dim_num[j];
                    }
                    mul_dim *= 4;
                    string _t;
                    new_temp(_t);
                    code_node _code;
                    _code.code_list.emplace_back(_t);
                    _code.code_list.emplace_back(":=");
                    _code.code_list.emplace_back(index_ir);
                    _code.code_list.emplace_back("*");
                    _code.code_list.emplace_back(to_string(mul_dim));
                    code.emplace_back(_code);

                    string _tmp_addr;
                    new_temp(_tmp_addr);
                    code_node _code_2;
                    _code_2.code_list.emplace_back(_tmp_addr);
                    _code_2.code_list.emplace_back(":=");
                    _code_2.code_list.emplace_back(off);
                    _code_2.code_list.emplace_back("+");
                    _code_2.code_list.emplace_back(_t);
                    code.emplace_back(_code_2);
                    off = _tmp_addr;
                }
                string _last_index;
                new_temp(_last_index);
                code_node _code_4;
                _code_4.code_list.emplace_back(_last_index);
                _code_4.code_list.emplace_back(":=");
                _code_4.code_list.emplace_back(ary_dim[0]);
                _code_4.code_list.emplace_back("*");
                _code_4.code_list.emplace_back("4");
                code.emplace_back(_code_4);

                new_temp(final_addr);
                code_node _code_3;
                _code_3.code_list.emplace_back(final_addr);
                _code_3.code_list.emplace_back(":=");
                _code_3.code_list.emplace_back(off);
                _code_3.code_list.emplace_back("+");
                _code_3.code_list.emplace_back(_last_index);
                code.emplace_back(_code_3);
            }
            else
            { //not full
                for (int i = 0; i < ary_dim.size(); i++)
                {
                    string index_ir = ary_dim[ary_dim.size() - i - 1];
                    int mul_dim = ary_val->dim_num[i + 1];
                    for (int j = i + 2; j < ary_val->dim; j++)
                    {
                        mul_dim *= ary_val->dim_num[j];
                    }
                    mul_dim *= 4;
                    string _t;
                    new_temp(_t);
                    code_node _code;
                    _code.code_list.emplace_back(_t);
                    _code.code_list.emplace_back(":=");
                    _code.code_list.emplace_back(index_ir);
                    _code.code_list.emplace_back("*");
                    _code.code_list.emplace_back(to_string(mul_dim));
                    code.emplace_back(_code);

                    string _tmp_addr;
                    new_temp(_tmp_addr);
                    code_node _code_2;
                    _code_2.code_list.emplace_back(_tmp_addr);
                    _code_2.code_list.emplace_back(":=");
                    _code_2.code_list.emplace_back(off);
                    _code_2.code_list.emplace_back("+");
                    _code_2.code_list.emplace_back(_t);
                    code.emplace_back(_code_2);
                    off = _tmp_addr;
                }
                final_addr = off;
            }

            res.code_list.emplace_back("*" + final_addr);
            res.code_list.emplace_back(":=");
            res.code_list.emplace_back(_t1);
        }
    }
    else if (exp->next_layer->next_neighbor->name == "PLUS" ||
             exp->next_layer->next_neighbor->name == "MINUS" ||
             exp->next_layer->next_neighbor->name == "MUL" ||
             exp->next_layer->next_neighbor->name == "DIV")
    {

        string _t1, _t2;
        struct ast *exp1 = exp->next_layer;
        struct ast *exp2 = exp->next_layer->next_neighbor->next_neighbor;

        exp_opt(exp1, _t1);
        exp_opt(exp2, _t2);

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

        string _t1;
        struct ast *_exp2 = exp->next_layer->next_neighbor;
        exp_opt(_exp2, _t1);
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
        string _label_1, _label_2;
        new_label(_label_1);
        new_label(_label_2);

        code_node _code_0;
        _code_0.code_list.emplace_back(place);
        _code_0.code_list.emplace_back(":=");
        _code_0.code_list.emplace_back("#0");
        code.emplace_back(_code_0);

        code.emplace_back(translate_cond(exp, _label_1, _label_2));

        code_node _code_1, _code_2;
        _code_1.code_list.emplace_back("LABRL");
        _code_1.code_list.emplace_back(_label_1);
        code.emplace_back(_code_1);

        _code_2.code_list.emplace_back(place);
        _code_2.code_list.emplace_back(":=");
        _code_2.code_list.emplace_back("#1");
        code.emplace_back(_code_2);

        res.code_list.emplace_back("LABEL");
        res.code_list.emplace_back(_label_2);
        res.code_list.emplace_back(":");
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
                if (place.size() == 0)
                {
                    new_temp(place);
                }
                res.code_list.emplace_back(place);
                res.code_list.emplace_back(":=");
                res.code_list.emplace_back("CALL");
                res.code_list.emplace_back(exp->next_layer->value);
            }
        }
        else
        {

            vector<string> arg_list;
            translate_args(exp->next_layer->next_neighbor->next_neighbor, arg_list);

            if (exp->next_layer->value == "write")
            {
                res.code_list.emplace_back("WRITE");
                res.code_list.emplace_back(arg_list[0]);
            }
            else
            {
                for (int i = arg_list.size() - 1; i > -1; i--)
                {
                    code_node _code;
                    _code.code_list.emplace_back("ARG");
                    _code.code_list.emplace_back(arg_list[i]);
                    code.emplace_back(_code);
                }
                if (place.size() == 0)
                {
                    new_temp(place);
                }
                res.code_list.emplace_back(place);
                res.code_list.emplace_back(":=");
                res.code_list.emplace_back("CALL");
                res.code_list.emplace_back(exp->next_layer->value);
            }
        }
    }

    return res;
}

code_node translate_stmt(struct ast *stmt)
{
    code_node res;
    if (stmt->next_layer->name == "Exp")
    {
        struct ast *tmp_exp = stmt->next_layer;
        while (tmp_exp->name != "ID")
        {
            tmp_exp = tmp_exp->next_layer;
        }
        string name = tmp_exp->value;
        val_d *val = val_find(name);
        if (val)
        {
            if (val->ir_name == "no_ir")
            {
                new_temp(val->ir_name);
            }
            res = (translate_exp(stmt->next_layer, val->ir_name));
        }
        else
        { //function
            res = (translate_exp(stmt->next_layer, ""));
        }
    }
    else if (stmt->next_layer->name == "CompSt")
    {
        function_def_list(stmt->next_layer->next_layer->next_neighbor);
        function_stmt_list(stmt->next_layer->next_layer->next_neighbor->next_neighbor);
    }
    else if (stmt->next_layer->name == "RETURN")
    {
        string _t1;
        exp_opt(stmt->next_layer->next_neighbor, _t1);
        res.code_list.emplace_back("RETURN");
        res.code_list.emplace_back(_t1);
    }
    else if (stmt->next_layer->name == "IF")
    { //if
        struct ast *_if = stmt->next_layer;
        if (_if->next_neighbor->next_neighbor->next_neighbor->next_neighbor->next_neighbor != NULL)
        { //if else
            string _label_1, _label_2, _label_3;
            new_label(_label_1);
            new_label(_label_2);
            new_label(_label_3);
            code.emplace_back(translate_cond(_if->next_neighbor->next_neighbor, _label_1, _label_2));

            code_node _code_4;
            _code_4.code_list.emplace_back("LABEL");
            _code_4.code_list.emplace_back(_label_1);
            _code_4.code_list.emplace_back(":");
            code.emplace_back(_code_4);

            code_node _code_2 = translate_stmt(_if->next_neighbor->next_neighbor->next_neighbor->next_neighbor);
            code.emplace_back(_code_2);

            code_node _code_5;
            _code_5.code_list.emplace_back("GOTO");
            _code_5.code_list.emplace_back(_label_3);
            code.emplace_back(_code_5);

            code_node _code_6;
            _code_6.code_list.emplace_back("LABEL");
            _code_6.code_list.emplace_back(_label_2);
            _code_6.code_list.emplace_back(":");
            code.emplace_back(_code_6);

            code_node _code_3 = translate_stmt(_if->next_neighbor->next_neighbor->next_neighbor->next_neighbor->next_neighbor->next_neighbor);
            code.emplace_back(_code_3);

            code_node _code_7;
            res.code_list.emplace_back("LABEL");
            res.code_list.emplace_back(_label_3);
            res.code_list.emplace_back(":");
        }
        else
        { //if
            string _label_1, _label_2;
            new_label(_label_1);
            new_label(_label_2);
            code.emplace_back(translate_cond(_if->next_neighbor->next_neighbor, _label_1, _label_2));

            code_node _code_3;
            _code_3.code_list.emplace_back("LABEL");
            _code_3.code_list.emplace_back(_label_1);
            _code_3.code_list.emplace_back(":");
            code.emplace_back(_code_3);

            code_node _code_2 = translate_stmt(_if->next_neighbor->next_neighbor->next_neighbor->next_neighbor);
            code.emplace_back(_code_2);

            res.code_list.emplace_back("LABEL");
            res.code_list.emplace_back(_label_2);
            res.code_list.emplace_back(":");
        }
    }
    else
    { //while
        struct ast *_while = stmt->next_layer;

        string _label_1, _label_2, _label_3;
        new_label(_label_1);
        new_label(_label_2);
        new_label(_label_3);

        code_node _code_3;
        _code_3.code_list.emplace_back("LABEL");
        _code_3.code_list.emplace_back(_label_1);
        _code_3.code_list.emplace_back(":");
        code.emplace_back(_code_3);

        code_node _code_1 = translate_cond(_while->next_neighbor->next_neighbor, _label_2, _label_3);

        code.emplace_back(_code_1);

        code_node label_2;
        label_2.code_list.emplace_back("LABEL");
        label_2.code_list.emplace_back(_label_2);
        label_2.code_list.emplace_back(":");
        code.emplace_back(label_2);

        code_node _code_2 = translate_stmt(_while->next_neighbor->next_neighbor->next_neighbor->next_neighbor);
        code.emplace_back(_code_2);

        code_node goto_label_1;
        goto_label_1.code_list.emplace_back("GOTO");
        goto_label_1.code_list.emplace_back(_label_1);
        code.emplace_back(goto_label_1);

        code_node label_3;
        label_3.code_list.emplace_back("LABEL");
        label_3.code_list.emplace_back(_label_3);
        label_3.code_list.emplace_back(":");
        code.emplace_back(label_3);
    }
    return res;
}

code_node translate_cond(struct ast *exp, string label_true, string label_false)
{
    code_node res;
    if (exp->next_layer->next_neighbor->name == "LT" ||
        exp->next_layer->next_neighbor->name == "LE" ||
        exp->next_layer->next_neighbor->name == "GT" ||
        exp->next_layer->next_neighbor->name == "GE" ||
        exp->next_layer->next_neighbor->name == "NE" ||
        exp->next_layer->next_neighbor->name == "EQ")
    {

        string _t1, _t2;
        exp_opt(exp->next_layer, _t1);
        exp_opt(exp->next_layer->next_neighbor->next_neighbor, _t2);

        code_node _true;
        _true.code_list.emplace_back("IF");
        _true.code_list.emplace_back(_t1);
        _true.code_list.emplace_back(exp->next_layer->next_neighbor->value);
        _true.code_list.emplace_back(_t2);
        _true.code_list.emplace_back("GOTO");
        _true.code_list.emplace_back(label_true);
        code.emplace_back(_true);

        res.code_list.emplace_back("GOTO");
        res.code_list.emplace_back(label_false);
    }
    else if (exp->next_layer->name == "NOT")
    {
        return translate_cond(exp->next_layer->next_neighbor, label_false, label_true);
    }
    else if (exp->next_layer->next_neighbor->name == "AND")
    {
        string label;
        new_label(label);
        code_node _code_1 = translate_cond(exp->next_layer, label, label_false);
        code.emplace_back(_code_1);

        code_node _label;
        _label.code_list.emplace_back("LABEL");
        _label.code_list.emplace_back(label);
        _label.code_list.emplace_back(":");
        code.emplace_back(_label);

        return translate_cond(exp->next_layer->next_neighbor->next_neighbor, label_true, label_false);
    }
    else if (exp->next_layer->next_neighbor->name == "OR")
    {
        string label;
        new_label(label);
        code_node _code_1 = translate_cond(exp->next_layer, label_true, label);
        code.emplace_back(_code_1);

        code_node _label;
        _label.code_list.emplace_back("LABEL");
        _label.code_list.emplace_back(label);
        _label.code_list.emplace_back(":");
        code.emplace_back(_label);

        return translate_cond(exp->next_layer->next_neighbor->next_neighbor, label_true, label_false);
    }
    else
    {
        string _t1;
        exp_opt(exp, _t1);
        code_node _label;
        _label.code_list.emplace_back("IF");
        _label.code_list.emplace_back(_t1);
        _label.code_list.emplace_back("!=");
        _label.code_list.emplace_back("#0");
        _label.code_list.emplace_back("GOTO");
        _label.code_list.emplace_back(label_true);
        code.emplace_back(_label);
        res.code_list.emplace_back("GOTO");
        res.code_list.emplace_back(label_false);
    }
    return res;
};

void translate_args(struct ast *args, vector<string> &arg_list)
{
    string _t1;
    struct ast *exp = args->next_layer;
    exp_opt(exp, _t1);
    arg_list.emplace_back(_t1); //tail
    if (args->next_layer->next_neighbor != NULL)
    {
        translate_args(args->next_layer->next_neighbor->next_neighbor, arg_list);
    }
}

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
            // cout<<p.name<<" == " + p.ir_name<<endl;
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

    while (def != NULL && def->name.size() > 0)
    {
        struct ast *dec = def->next_layer->next_neighbor->next_layer;
        while (dec != NULL && dec->name.size() > 0)
        { //int a,b,c,d=10;
            struct ast *var_dec = dec->next_layer;
            if (var_dec->next_neighbor == NULL && var_dec->next_layer->name == "VarDec")
            { //int a[][]
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
            else if (var_dec->next_neighbor != NULL)
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
                dec = dec->next_neighbor->next_neighbor->next_layer;
            }
            else
            {
                break;
            }
        }

        def = def->next_neighbor->next_layer;
    }
}

void function_stmt_list(struct ast *stmt_list)
{
    struct ast *stmt = stmt_list->next_layer;
    while (stmt != NULL && stmt->name.size() > 0)
    {

        if (stmt->next_layer->name != "CompSt")
        {
            code.emplace_back(translate_stmt(stmt));
        }
        else
        {
            translate_stmt(stmt);
        }
        stmt = stmt->next_neighbor->next_layer;
    }
};

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
