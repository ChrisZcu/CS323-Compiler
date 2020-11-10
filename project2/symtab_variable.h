#ifndef VARIABLE_H
#define VARIABLE_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define KEY_LEN 32

typedef struct symtab_variable symtab_variable;


/* symbol table entry, only used internally */
typedef struct variable_symbol{
    char *type;
}VAL_VARIABLE;

typedef struct entry {
    char *key; //name
    VAL_VARIABLE value; //type
} entry;


void variable_entry_init(entry *self, char *key, VAL_VARIABLE value){
    sprintf(self->key, "%s", key);
    self->value = value;
}


// init a single symbol table
symtab_variable *variable_symtab_init();

// insert a key-value pair to the table
// if insert success, return 1, otherwise 0
int variable_symtab_insert(symtab_variable*, char*, VAL_VARIABLE);

// lookup the value of a specific key
// return -1 if not found
VAL_VARIABLE variable_symtab_lookup(symtab_variable*, char*);

// remove a key-value pair from the table
// if remove success, return 1, otherwise 0
int variable_symtab_remove(symtab_variable*, char*);

#endif  /* SYMTAB_H */