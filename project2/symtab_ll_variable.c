#include "symtab_variable.h"

/*
 * symbol table type, linked list impl
 */
typedef struct symtab_variable
{
    entry entry;
    struct symtab_variable *next;
}symtab_variable;

// ************************************************************
//    Your implementation goes here
// ************************************************************

symtab_variable *variable_symtab_init()
{
    symtab_variable *self = malloc(sizeof(symtab_variable));
    memset(self, '\0', sizeof(symtab_variable));
    self->next = NULL;
    return self;
}

int variable_symtab_insert(symtab_variable *self, char *key, VAL_VARIABLE value)
{
    symtab_variable *ptr = self;
    while (ptr->next != NULL)
    {
        if (strcmp(ptr->entry.key, key) == 0)
            return 0;
        ptr = ptr->next;
    }
    symtab_variable *node = malloc(sizeof(symtab_variable));
    memset(node, '\0', sizeof(symtab_variable));
    variable_entry_init(&node->entry, key, value);
    node->next = NULL;
    ptr->next = node;
    return 1;
}

VAL_VARIABLE variable_symtab_lookup(symtab_variable *self, char *key)
{
    symtab_variable *ptr = self;
    while (ptr != NULL)
    {
        if (strcmp(ptr->entry.key, key) == 0)
            return ptr->entry.value;
        ptr = ptr->next;
    }
    VAL_VARIABLE null_tmp;
    return null_tmp;
}

int variable_symtab_remove(symtab_variable *self, char *key)
{
    symtab_variable *ptr = self, *tmp;
    while (ptr->next != NULL)
    {
        if (strcmp(ptr->next->entry.key, key) == 0)
        {
            tmp = ptr->next;
            ptr->next = ptr->next->next;
            free(tmp);
            return 1;
        }
        ptr = ptr->next;
    }
    return 0;
}

// int main(){
//     return 0;
// }