#include "symtab.h"
#include <stdio.h>

/*
 * symbol table type, binary tree impl
 */
typedef struct symtab
{
    entry entry;
    struct symtab *left, *right;
} symtab;

// ************************************************************
//    Your implementation goes here
// ************************************************************

symtab *symtab_init()
{
    symtab *self = malloc(sizeof(symtab));
    memset(self, '\0', sizeof(symtab));
    self->left = NULL;
    self->right = NULL;
    return self;
}

int symtab_insert(symtab *self, char *key, VAL_T value)
{
    symtab *tmp_node = self, *tmp_last = self;

    while (tmp_node != NULL)
    {
        if (strcmp(tmp_node->entry.key, key) == 0)
        {
            // entry_init(&tmp_node->entry, key, value);
            // printf("res: %d\n", -1);
            // tmp_node->entry.value = value;
            return 0;
        }
        tmp_last = tmp_node;
        tmp_node = tmp_node->left;
    }

    symtab *node = malloc(sizeof(symtab));
    memset(node, '\0', sizeof(symtab));
    entry_init(&node->entry, key, value);
    // printf("eaqual %d\n", strcmp(key, "test2"));
    node->left = NULL;
    node->right = NULL;

    tmp_last->left = node;

    return 1;
}

VAL_T symtab_lookup(symtab *self, char *key)
{
    symtab *tmp_node = self;
    int i = 0;
    while (tmp_node != NULL)
    {
        if (strcmp(tmp_node->entry.key, key) == 0)
        {
            return tmp_node->entry.value;
        }
        tmp_node = tmp_node->left;
        i++;
    }
    return -1;
}

int symtab_remove(symtab *self, char *key)
{
    symtab *tmp_node = self;
    while (tmp_node->left != NULL)
    {
        if (strcmp(tmp_node->left->entry.key, key) == 0)
        {
            tmp_node->left = tmp_node->left->left;
            return 1;
        }
        tmp_node = tmp_node->left;
    }
    return 0;
}

int main()
{
    symtab *root = symtab_init();
    char *key = "test";
    char *key1 = "test1";
    char *key2 = "test2";

    symtab_insert(root, key, 1);
    symtab_insert(root, key1, 2);
    symtab_insert(root, key2, 3);
    symtab_insert(root, key, 10);
    symtab_insert(root, key2, 10);

    int res = symtab_lookup(root, key2);
    printf("res: %d\n", res);

    symtab_remove(root, key2);
    res = symtab_lookup(root, key2);

    printf("res: %d\n", res);

    return 0;
}