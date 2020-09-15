#include "linked_list.h"

node *linked_list_init()
{
    node *head = (node *)malloc(sizeof(node));
    head->count = 0;
    head->next = NULL;
    return head;
}

void linked_list_free(node *head)
{
    node *cur = head;
    node *last;
    while (cur != NULL)
    {
        last = cur;
        cur = cur->next;
        free(last);
    }
}

char linked_list_string[0x10000];

char *linked_list_tostring(node *head)
{
    node *cur = head->next;
    char *position;
    int length = 0;
    while (cur != NULL)
    {
        position = linked_list_string + length;
        length += sprintf(position, "%d", cur->value);
        cur = cur->next;
        if (cur != NULL)
        {
            position = linked_list_string + length;
            length += sprintf(position, "->");
        }
    }
    position = linked_list_string + length;
    length += sprintf(position, "%c", '\0');
    return linked_list_string;
}

int linked_list_size(node *head)
{
    return head->count;
}

void linked_list_append(node *head, int val)
{
    node *cur = head;
    node *new_node;
    while (cur->next != NULL)
    {
        cur = cur->next;
    }
    new_node = (node *)malloc(sizeof(node));
    new_node->value = val;
    new_node->next = NULL;
    cur->next = new_node;
    head->count++;
}

/* your implementation goes here */
int check_index(node *head, int index)
{
    if (index < 0 || index >= head->count)
    {
        return 1;
    }
    return 0;
}

void linked_list_insert(node *head, int val, int index)
{
    if (check_index(head, index))
    {
        return;
    }

    node *cur = head;
    while (index--)
    {
        cur = cur->next;
    }
    node *tmp_node = (node *)malloc(sizeof(node));
    tmp_node->value = val;
    tmp_node->next = cur->next;
    cur->next = tmp_node;
    head->count++;
}

void linked_list_delete(node *head, int index)
{
    if (check_index(head, index))
    {
        return;
    }

    node *cur = head;
    node *last_node;
    while (index--)
    {
        cur = cur->next;
    }
    last_node = cur->next;
    cur->next = last_node->next;
    head->count--;
    free(last_node);
}

void linked_list_remove(node *head, int val)
{
    node *last_node = head;
    node *cur;
    while (last_node->next != NULL)
    {
        cur = last_node->next;
        if (cur->value == val)
        {
            last_node->next = cur->next;
            head->count--;
            free(cur);
            return;
        }
        last_node = last_node->next;
    }
}

void linked_list_remove_all(node *head, int val)
{
    node *last_node = head;
    node *cur;
    while (last_node->next != NULL)
    {
        cur = last_node->next;
        if (cur->value == val)
        {
            last_node->next = cur->next;
            head->count--;
            free(cur);
        }
        else
            last_node = last_node->next;
    }
}

int linked_list_get(node *head, int index)
{
    if (check_index(head, index))
    {
        return -0x80000000;
    }

    node *cur = head;
    while (index--)
    {
        cur = cur->next;
    }
    return cur->next->value;
}

int linked_list_search(node *head, int val)
{
    node *cur = head;
    int index = 0;
    while (cur->next != NULL)
    {
        if (cur->next->value == val)
        {
            return index;
        }
        index++;
        cur = cur->next;
    }
    return -1;
}

node *linked_list_search_all(node *head, int val)
{
    node *new_index_list = linked_list_init();
    node *cur = head;
    int index = 0;
    while (cur->next != NULL)
    {
        if (cur->next->value == val)
        {
            linked_list_append(new_index_list, index);
        }
        index++;
        cur = cur->next;
    }
    return new_index_list;
}