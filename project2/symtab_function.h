#ifndef FUNTION_H
#define FUNTION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEY_LEN 32

typedef struct symtab_function symtab_function;

/* symbol table entry, only used internally */

typedef struct function_symbol
{
    char *return_type;
    struct ast *parameters;
} VAL_FUNTION;

typedef struct function_entry
{
    char key[KEY_LEN + 1];
    VAL_FUNTION value;
} entry_function;

void function_entry_init(entry_function *self, char *key, VAL_FUNTION value)
{   
    sprintf(self->key, "%s", key);
    self->value = value;
}

// init a single symbol table
symtab_function *function_symtab_init();

// insert a key-value pair to the table
// if insert success, return 1, otherwise 0
int function_symtab_insert(symtab_function *, char *, VAL_FUNTION);

// lookup the value of a specific key
// return -1 if not found
VAL_FUNTION function_symtab_lookup(symtab_function *, char *);

// remove a key-value pair from the table
// if remove success, return 1, otherwise 0
int function_symtab_remove(symtab_function *, char *);

#endif /* SYMTAB_H */
