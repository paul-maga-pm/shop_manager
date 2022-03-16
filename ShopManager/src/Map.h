#ifndef MAP_H
#define MAP_H

typedef struct map_struct map_t;

map_t*				map_create(void* (*value_copy_constructor)(void*),
                    void (*value_destructor)(void*));

void				map_destroy(map_t* map);
map_t*				map_copy(map_t* other);

void				map_put(map_t* map, char* key, void* value);
void*				map_get(map_t* map, char* key);

#endif 
