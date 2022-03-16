#include "CommandMap.h"

#include "Map.h"

#include <stdlib.h>
#include <string.h>



struct command_map_struct
{
    map_t* map;
};


command_map_t* command_map_create()
{
    command_map_t* command_map = (command_map_t*)calloc(1, sizeof(command_map_t));
    if (!command_map)
    {
        perror("Out of heap memory");
        exit(1);
    }
    command_map->map = map_create(NULL, NULL);
    return command_map;
}

void command_map_destroy(command_map_t* map)
{
    map_destroy(map->map);
    free(map);
    map = NULL;
}


void command_map_put(command_map_t* map, char* key, void(*command)(console_interface_t*))
{
    map_put(map->map, key, command);
}

void command_map_run(console_interface_t* console_interface, command_map_t* map, char* key)
{
    void (*command)(console_interface_t*) = map_get(map->map, key);

    if (command)
        command(console_interface);
}
