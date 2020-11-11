#include "symtab_function.h"

/*
 * symbol table type, linked list impl
 */
typedef struct symtab_function
{
    entry_function entry;
    struct symtab_function *next;
} symtab_function;

// ************************************************************
//    Your implementation goes here
// ************************************************************

symtab_function *function_symtab_init()
{
    symtab_function *self = malloc(sizeof(symtab_function));
    memset(self, '\0', sizeof(symtab_function));
    self->next = NULL;
    return self;
}

int function_symtab_insert(symtab_function *self, char *key, VAL_FUNTION value)
{
    symtab_function *ptr = self;
    while (ptr->next != NULL)
    {
        if (strcmp(ptr->entry.key, key) == 0)
            return 0;
        ptr = ptr->next;
    }
    symtab_function *node = malloc(sizeof(symtab_function));
    memset(node, '\0', sizeof(symtab_function));
    function_entry_init(&ptr->entry, key, value);
    node->next = NULL;
    ptr->next = node;

    return 1;
}

VAL_FUNTION function_symtab_lookup(symtab_function *self, char *key)
{
    symtab_function *ptr = self;
    while (ptr != NULL)
    {
        if (strcmp(ptr->entry.key, key) == 0)
            return ptr->entry.value;
        ptr = ptr->next;
    }
    VAL_FUNTION val_tmp;
    val_tmp.return_type = "null_false";
    return val_tmp;
}

int function_symtab_remove(symtab_function *self, char *key)
{
    symtab_function *ptr = self, *tmp;
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
