#ifndef CONSOLE_INTERFACE_H
#define CONSOLE_INTERFACE_H

#include "ShopService.h"

typedef struct console_interface_struct console_interface_t;

console_interface_t*	console_interface_create(shop_service_t* shop_service);
void					console_interface_destroy(console_interface_t* console_interface);

void					console_interface_run(console_interface_t* console_interface);

#endif 

