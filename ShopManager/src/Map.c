#include "Map.h"


#include "List.h"

#include <stdlib.h>
#include <string.h>

#define HASH_TABLE_SIZE 50


typedef struct map_entry_struct
{
    char* key;
    void* value;
    void*(*value_copy_constructor)(void*);
    void (*value_destructor)(void*);
}map_entry_t;

map_entry_t* map_entry_create(char* key, void* value, void* (*value_copy_constructor)(void*),
                            void (*value_destructor)(void*))
{
    map_entry_t* entry = (map_entry_t*)calloc(1, sizeof(map_entry_t));
    if (!entry)
    {
        perror("Out of heap memory");
        exit(1);
    }

    size_t key_size = strlen(key);
    char* key_copy = (char*)calloc(key_size + 1, sizeof(char));
    if (!key_copy)
    {
        perror("Out of heap memory");
        exit(1);
    }
    strcpy_s(key_copy, key_size + 1, key);
    entry->key = key_copy;
    entry->value_destructor = value_destructor;
    entry->value_copy_constructor = value_copy_constructor;

    if (!value_copy_constructor)
        entry->value = value;
    else
        entry->value = value_copy_constructor(value);
    return entry;
}


map_entry_t* map_entry_copy(map_entry_t* other)
{
    if (!other)
        return NULL;
    return map_entry_create(other->key, other->value, other->value_copy_constructor, other->value_destructor);
}

void map_entry_destroy(map_entry_t* entry)
{
    free(entry->key);
    entry->key = NULL;
    if (entry->value_destructor)
        entry->value_destructor(entry->value);
    free(entry);
    entry = NULL;
}


int map_entry_have_same_key(map_entry_t* entry1, map_entry_t* entry2)
{
    return !strcmp(entry1->key, entry2->key);
}

int map_entry_has_key(map_entry_t* entry, char* key)
{
    return !strcmp(entry->key, key);
}

struct map_struct
{
    list_t* command_hash_table[HASH_TABLE_SIZE];
    void* (*value_copy_constructor)(void*);
    void (*value_destructor)(void*);
};


map_t* map_create(
    void* (*value_copy_constructor)(void*),
    void (*value_destructor)(void*))
{
    map_t* map = (map_t*)calloc(1, sizeof(map_t));
    if (!map)
    {
        perror("Out of heap memory");
        exit(1);
    }
    for (size_t i = 0; i < HASH_TABLE_SIZE; i++)
        map->command_hash_table[i] = list_create(map_entry_copy, map_entry_destroy);

    map->value_copy_constructor = value_copy_constructor;
    map->value_destructor = value_destructor;
    return map;
}

map_t* map_copy(map_t* other)
{
    map_t* map = (map_t*)calloc(1, sizeof(map_t));
    if (!map)
    {
        perror("Out of heap memory");
        exit(1);
    }
    for (size_t i = 0; i < HASH_TABLE_SIZE; i++)
        map->command_hash_table[i] = list_copy(other->command_hash_table[i]);

    map->value_copy_constructor = other->value_copy_constructor;
    map->value_destructor = other->value_destructor;
    return map;
}

void map_destroy(map_t* map)
{
    for (int i = 0; i < HASH_TABLE_SIZE; i++)
        list_destroy(map->command_hash_table[i]);
    free(map);
    map = NULL;
}

long long int hash_code(char* key)
{
    long long int hash_code = 0;

    for (size_t i = 0; key[i]; i++)
        hash_code = hash_code * 31 + key[i];
    return hash_code;
}

int hash(char* key)
{
    return hash_code(key) % HASH_TABLE_SIZE;
}


void map_put(map_t* map, char* key, void* value)
{
    int index = hash(key);

    list_iterator_t* iterator_to_existing_key = list_find_if(map->command_hash_table[index],
        key,
        map_entry_has_key);

    if (!list_iterator_is_valid(iterator_to_existing_key))
    {
        map_entry_t* entry = map_entry_create(key, value, map->value_copy_constructor, map->value_destructor);
        list_add(map->command_hash_table[index], entry);
        map_entry_destroy(entry);
    }
    list_iterator_destroy(iterator_to_existing_key);
}

void* map_get(map_t* map, char* key)
{
    int index = hash(key);

    list_iterator_t* iterator_to_existing_key = list_find_if(map->command_hash_table[index],
        key,
        map_entry_has_key);

    void* value = NULL;
    if (list_iterator_is_valid(iterator_to_existing_key))
    {
        map_entry_t* entry = list_iterator_get(iterator_to_existing_key);
        value = entry->value;
    }
    list_iterator_destroy(iterator_to_existing_key);
    return value;
}
