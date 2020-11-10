typedef struct struct_json
{
    char *name;
    union
    {
        char *value;
        struct array *array;
        struct struct_json *json;
    };

} struct_json;

typedef struct array
{
    struct struct_json *json_array;
    int size;
}array;

int main()
{
    return 0;
}