#include "Character.h"

#include <stdlib.h>
struct character_struct
{
    char value;
};

character_t* character_create(char value)
{
    character_t* c = (character_t*)calloc(1, sizeof(character_t));
    if (!c)
    {
        perror("Out of heap memory");
        exit(1);
    }
    c->value = value;
    return c;
}

void character_destroy(character_t* character)
{
    free(character);
    character = NULL;
}

character_t* character_copy(character_t* other)
{
    return character_create(other->value);
}

char character_get_value(character_t* character)
{
    return character->value;
}

void character_set_value(character_t* character, char newValue)
{
    character->value = newValue;
}
