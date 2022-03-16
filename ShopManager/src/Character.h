#ifndef CHARACTER_H
#define CHARACTER_H

typedef struct character_struct character_t;

character_t*	character_create(char value);
void			character_destroy(character_t* character);
character_t*	character_copy(character_t* other);
char			character_get_value(character_t* character);
void			character_set_value(character_t* character, char newValue);

#endif 
