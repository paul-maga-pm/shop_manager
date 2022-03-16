#ifndef CONSOLE_INTERFACE_COMMAND_MAP_H
#define CONSOLE_INTERFACE_COMMAND_MAP_H

#include "ConsoleInterface.h"

typedef struct command_map_struct command_map_t;

command_map_t*		command_map_create();
void				command_map_destroy(command_map_t* map);

void				command_map_put(command_map_t* map, char* key, void(*command)(console_interface_t*));
void				command_map_run(console_interface_t* console_interface, command_map_t* map, char* key);

#endif 
