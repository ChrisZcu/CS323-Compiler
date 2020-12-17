#include <string>
#include <vector>
#include <iostream>

#include "symtable.hpp"

using namespace std;

struct symbol
{
    vector<func_d> func;
    vector<val_d> val; //val,ary,struct
} symbol;

void symbal_init()
{
    struct func_d func_read, func_write;
    func_read.name = "read";
    func_write.name = "write";

    symbol.func.emplace_back(func_read);
    symbol.func.emplace_back(func_write);
}

func_d *func_find(string name)
{
    for (int i = 0; i < symbol.func.size(); i++)
    {
        if (symbol.func[i].name == name)
        {
            return &symbol.func[i];
        }
    }
    return NULL;
}

val_d *val_find(string name)
{
    for (int i = 0; i < symbol.val.size(); i++)
    {
        if (symbol.val[i].name == name)
        {
            return &symbol.val[i];
        }
    }
    return NULL;
}