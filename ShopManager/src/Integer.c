#include "Integer.h"

#include <stdlib.h>

struct integer_struct
{
    int value;
};

integer_t* integer_create(int value)
{
    integer_t* integer = (integer_t*)calloc(1, sizeof(integer_t));
    if (!integer)
    {
        perror("Out of heap memory");
        exit(1);
    }
    integer->value = value;
    return integer;
}

void integer_destroy(integer_t* integer)
{
    free(integer);
    integer = NULL;
}

integer_t* integer_copy(integer_t* other)
{
    return integer_create(other->value);
}

int integer_get_value(integer_t* integer)
{
    return integer->value;
}
