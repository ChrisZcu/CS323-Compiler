#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef LL_VARIABLE
#define LL_VARIABLE
#include "symtab_ll_variable.c"
#endif

#ifndef LL_FUNCTION
#define LL_FUNCTION
#include "symtab_ll_function.c"
#endif

int main(){
    
    VAL_VARIABLE value;
    for (int i = 0; i < 10; i++)
    {
        sprintf(value.IR_name , "%s%d", "t", i);
        printf("%s\n", value.IR_name);
    }
    return 0;
}