#include "List.h"

#include <stdlib.h>

#include "Constants.h"

typedef struct node_struct
{
    struct node_struct* previous;
    struct node_struct* next;
    void* data;
}node_t;


node_t* create_node(node_t* previous, node_t* next, void* data)
{
    node_t* node = (node_t*)calloc(1, sizeof(node_t));
    if (!node)
    {
        perror("Out of heap memory\n");
        exit(1);
    }
    node->previous = previous;
    node->next = next;
    node->data = data;
    return node;
}

struct list_struct
{
    node_t* head;
    node_t* tail;
    size_t size;
    void* (*element_copy_constructor)(void*);
    void (*element_destructor)(void*);
};


typedef struct list_iterator_struct
{
    list_t* list_reference;
    node_t* current;
}list_iterator_t;


list_t* list_create(void*(*element_copy_constructor)(void*), 
    void(*element_destructor)(void*))
{
    list_t* list = (list_t*)calloc(1, sizeof(list_t));
    if (!list)
    {
        perror("Out of heap memory\n");
        exit(1);
    }
    list->size = 0;
    list->head = NULL;
    list->tail = NULL;
    list->element_copy_constructor = element_copy_constructor;
    list->element_destructor = element_destructor;
    return list;
}

void list_destroy(list_t* list)
{
    node_t* current_node = list->head;

    while (current_node)
    {
        node_t* temp = current_node->next;
        if(list->element_destructor)
            list->element_destructor(current_node->data);
        free(current_node);
        current_node = temp;
    }

    list->size = 0;
    list->head = list->tail = NULL;
    free(list);
    list = NULL;
}

list_t* list_copy(list_t* other)
{
    list_t* copy_of_other = list_create(other->element_copy_constructor, other->element_destructor);
    node_t* current = other->head;
    while (current)
    {
        list_add(copy_of_other, current->data);
        current = current->next;
    }
    return copy_of_other;
}

size_t list_size(list_t* list)
{
    return list->size;
}

void list_add(list_t* list, void* element)
{
    void* value;

    if (!list->element_copy_constructor)
        value = element;
    else
        value = list->element_copy_constructor(element);

    node_t* new_node = create_node(NULL, NULL, value);
    list->size++;
    if (!list->head)
    {
        list->head = list->tail = new_node;
        return;
    }

    if (list->head == list->tail)
    {
        list->tail = new_node;
        list->head->next = list->tail;
        list->tail->previous = list->head;
        return;
    }

    list->tail->next = new_node;
    new_node->previous = list->tail;
    list->tail = new_node;
}

list_iterator_t* list_find_if(list_t* list, void* value, int(*comparator)(void* current_element, void* value))
{
    list_iterator_t* iterator = list_iterator_create(list);

    while (list_iterator_is_valid(iterator))
    {
        if (comparator(list_iterator_get(iterator), value))
            return iterator;
        list_iterator_next(iterator);
    }
    return iterator;
}

void list_for_each(list_t* list, void(*consumer)(void*))
{
    node_t* current = list->head;
    while (current)
    {
        consumer(current->data);
        current = current->next;
    }
}

int list_remove_if(list_t* list, void* value, int(*comparator)(void* current_element, void* value))
{
    if (!list->head)
        return false;

    node_t* current;
    
    for (current = list->head; current; current = current->next)
        if (comparator(current->data, value))
            break;

    if (!current)
        return false;

    if (list->element_destructor)
        list->element_destructor(current->data);
    
    list->size--;
    
    if(current == list->head)
        if (list->head == list->tail)
            list->head = list->tail = NULL;
        else
        {
            current->next->previous = NULL;
            list->head = current->next;
        }
    else if(current == list->tail)
    {
        list->tail = current->previous;
        list->tail->next = NULL;
    }
    else
    {
        current->previous->next = current->next;
        current->next->previous = current->previous;
    }
    free(current);
    return true;
}

list_t* list_copy_if(list_t* list, void* value, int(*comparator)(void* current_element, void* value))
{
    if (!list)
        return NULL;

    list_t* out_list = list_create(list->element_copy_constructor, list->element_destructor);

    for (node_t* current = list->head; current; current = current->next)
        if (comparator(current->data, value))
            list_add(out_list, current->data);
    return out_list;
}

void list_sort(list_t* list, int(*comparator)(void* element1, void* element2))
{
    if (!list->head)
        return;

    int is_done = false;

    while (!is_done)
    {
        is_done = true;

        for(node_t* i = list->head; i->next; i = i->next)
            if (!comparator(i->data, i->next->data))
            {
                void* temp = i->data;
                i->data = i->next->data;
                i->next->data = temp;
                is_done = false;
            }
    }
}

void* list_get(list_t* list, size_t index)
{
    size_t i = 0;
    node_t* current = list->head;

    while (current && i != index)
    {
        current = current->next;
        i++;
    }

    if (current)
        return current->data;
    return NULL;
}


list_iterator_t* list_iterator_create(list_t* list)
{
    list_iterator_t* iterator = (list_iterator_t*)calloc(1, sizeof(list_iterator_t));
    if (!iterator)
    {
        perror("Out of heap memory");
        exit(1);
    }
    
    iterator->current = list->head;
    iterator->list_reference = list;
    return iterator;
}

void list_iterator_destroy(list_iterator_t* iterator)
{
    free(iterator);
    iterator = NULL;
}

int list_iterator_is_valid(list_iterator_t* iterator)
{
    if (!iterator->current)
        return false;
    return true;
}

int list_iterator_next(list_iterator_t* iterator)
{
    int is_iterator_valid = list_iterator_is_valid(iterator);
    if (is_iterator_valid)
        iterator->current = iterator->current->next;
    return is_iterator_valid;
}

int list_iterator_previous(list_iterator_t* iterator)
{
    int is_iterator_valid = list_iterator_is_valid(iterator);
    if (is_iterator_valid)
        iterator->current = iterator->current->previous;
    return is_iterator_valid;
}

void* list_iterator_get(list_iterator_t* iterator)
{
    if (list_iterator_is_valid(iterator))
        return iterator->current->data;
    return NULL;
}

void list_iterator_first(list_iterator_t* iterator)
{
    iterator->current = iterator->list_reference->head;
}

void list_iterator_last(list_iterator_t* iterator)
{
    iterator->current = iterator->list_reference->tail;
}








