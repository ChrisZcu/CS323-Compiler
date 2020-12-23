#include <string>
#include <vector>

using namespace std;

typedef struct val_d
{
    string name;
    string type;         //int,float,char,struct(the name itself)
    int dim = 0;         //ary used
    vector<int> dim_num; //ary used
    bool is_func_args = false;
    int value = 0;           //int used

    vector<val_d> struct_field; //struct field used
    string ir_name="no_ir";        //ir code used
} val_d;

typedef struct func_d
{
    string name;
    vector<val_d> args;
    string return_type;
} func_d;

void symbal_init();
func_d *func_find(string name);
val_d *val_find(string name);
void func_struct_insert(struct ast *ast);
void local_variable_insert(struct ast *ast, vector<val_d> &val_vec);
void dec_list_charge(string type, struct ast *ast, vector<val_d> &val_vec);
void dec_charge(string type, struct ast *ast, vector<val_d> &val_vec);
int var_dec_charge(string type, struct ast *ast, vector<val_d> &val_vec);
void func_args(struct ast *var_list, vector<val_d> &args);
val_d get_val(string type, struct ast *ast);
void struct_variable_insert(struct ast *ast, vector<val_d> &val_vec);
