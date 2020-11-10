#ifndef STDARG_H
#define STDARG_H
#include <stdarg.h>
#endif
#ifndef STDLIB_H
#define STDLIB_H
#include <stdlib.h>
#endif
#include <string.h>
#include <stdio.h>


struct JsonObject
{
    union
    {
        struct ObjectMember *members;
        struct ArrayValue *values;
        char *string;
        double number;
        int boolean;
    };
};

struct ObjectMember
{
    char *key;
    struct JsonObject *value;
    struct ObjectMember *next;
};

struct ArrayValue
{
    struct JsonObject *value;
    struct ArrayValue *next;
};

struct JsonObject *new_json_object(struct ObjectMember *member)
{
    struct JsonObject *json_value = (struct JsonObject *)malloc(sizeof(struct JsonObject));
    json_value->members = member;
    return json_value;
}

// struct JsonObject *new_json_init_object(struct ObjectMember *member)
// {
//     struct ObjectMember *tmp_member = new_object_member($1, $3, NULL);

//     struct JsonObject *json_value = (struct JsonObject *)malloc(sizeof(struct JsonObject));
//     json_value->members = member;
//     return json_value;
// }

struct JsonObject *json_add_object(struct JsonObject *member, struct JsonObject *head)
{
    head->members->next = member->members;
    return head;
}

struct JsonObject *new_json_array(struct ArrayValue *values)
{
    struct JsonObject *json_value = (struct JsonObject *)malloc(sizeof(struct JsonObject));
    json_value->values = values;
    return json_value;
}

struct JsonObject *new_json_string(char *string)
{
    struct JsonObject *json_value = (struct JsonObject *)malloc(sizeof(struct JsonObject));
    json_value->string = string;
    return json_value;
}

struct JsonObject *new_json_number(double number)
{
    struct JsonObject *json_value = (struct JsonObject *)malloc(sizeof(struct JsonObject));
    json_value->number = number;
    return json_value;
}

struct JsonObject *new_json_bool(int boolean)
{
    struct JsonObject *json_value = (struct JsonObject *)malloc(sizeof(struct JsonObject));
    json_value->boolean = boolean;
    return json_value;
}

struct JsonObject *new_json_vnull()
{
    struct JsonObject *json_value = (struct JsonObject *)malloc(sizeof(struct JsonObject));
    return json_value;
}

struct ObjectMember *new_object_member(char *key, struct JsonObject *value, struct ObjectMember *next)
{
    struct ObjectMember *json_object = (struct ObjectMember *)malloc(sizeof(struct ObjectMember));
    json_object->key = key;
    json_object->value = value;
    json_object->next = next;
    return json_object;
}

struct ObjectMember *new_object_member_(struct JsonObject *value)
{
    return value->members;
}

struct ArrayValue *new_array_value(struct JsonObject *value)
{
    struct ArrayValue *array_value_tmp = (struct ArrayValue *)malloc(sizeof(struct ArrayValue));
    array_value_tmp->value = value;
}

int check_key(struct JsonObject *members)
{   
    char *c = '.';
    printf("%s", c);

    struct ObjectMember *tmp = members->members;
    while (tmp->next != NULL)
    {
        printf("%s", c);
        if (strstr(c, tmp->key))
        {
            printf("duplicate key: \"%s\"", tmp->key);
            return 1;
        }
        c = strcpy(c, tmp->key);
        tmp = tmp->next;
    }
}

int main()
{

    return 1;
}