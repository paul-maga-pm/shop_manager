
/*
	int id;
	char* type;
	char* manufacturer;
	char* model;
	float price;
	int quantity;
*/
#include <stdio.h>
#include "List.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <stdio.h>

#include "ShopService.h"
#include "ConsoleInterface.h"
#include "List.h"

#include "Integer.h"
void print_integer(integer_t* integer)
{
	printf("%d ", integer_get_value(integer));
}

int comp(integer_t* integer, int* value)
{
	return integer_get_value(integer) == *value;
}

int compare(integer_t* int1, integer_t* int2)
{
	return integer_get_value(int1) <= integer_get_value(int2);
}

#include "Map.h"
void test_remove()
{
	integer_t* n1 = integer_create(1);
	integer_t* n2 = integer_create(2);
	integer_t* n3 = integer_create(3);

	map_t* map = map_create(integer_copy, integer_destroy);
	map_put(map, "unu", n1);
	map_put(map, "doi", n2);
	map_put(map, "trei", n3);

	map_t* copy = map_copy(map);
	map_destroy(map);
	integer_destroy(n1);
	integer_destroy(n2);
	integer_destroy(n3);
	map_destroy(copy);
}

void main()
{
	//test_remove();
	shop_service_t* shop_service = shop_service_create();
	console_interface_t* console_interface = console_interface_create(shop_service);

	console_interface_run(console_interface);

	shop_service_destroy(shop_service);
	console_interface_destroy(console_interface);
	_CrtDumpMemoryLeaks();
}
