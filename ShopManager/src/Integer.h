#ifndef INTEGER_H
#define INTEGER_H

typedef struct integer_struct integer_t;

integer_t*	integer_create(int value);
void		integer_destroy(integer_t* integer);
integer_t*	integer_copy(integer_t* other);

int			integer_get_value(integer_t* integer);

#endif 

