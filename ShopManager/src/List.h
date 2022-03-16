#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

typedef struct list_struct list_t;
typedef struct list_iterator_struct list_iterator_t;

list_t*				list_create(void*(*element_copy_constructor)(void*),
					void (*element_destructor)(void*));
void				list_destroy(list_t* list);
list_t*				list_copy(list_t* other);

size_t				list_size(list_t* list);
void				list_add(list_t* list, void* element);
list_iterator_t*	list_find_if(list_t* list, void* value, int(*comparator)(void* current_element, void* value));
void				list_for_each(list_t* list, void(*consumer)(void*));
int					list_remove_if(list_t* list, void* value, int(*comparator)(void* current_element, void* value));
list_t*				list_copy_if(list_t* list, void* value, int(*comparator)(void* current_element, void* value));

void				list_sort(list_t* list, int(*comparator)(void* element1, void* element2));
void*				list_get(list_t* list, size_t index);

list_iterator_t*	list_iterator_create(list_t* list);
void				list_iterator_destroy(list_iterator_t* iterator);
int					list_iterator_is_valid(list_iterator_t* iterator);
int					list_iterator_next(list_iterator_t* iterator);
int					list_iterator_previous(list_iterator_t* iterator);
void*				list_iterator_get(list_iterator_t* iterator);
void				list_iterator_first(list_iterator_t* iterator);
void				list_iterator_last(list_iterator_t* iterator);

#endif