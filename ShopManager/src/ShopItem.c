#include "ShopItem.h"

#include "Constants.h"

#include <stdlib.h>
#include <string.h>

struct shop_item_struct
{
	int id;
	char* type;
	char* manufacturer;
	char* model;
	double price;
	int quantity;
};

shop_item_t* shop_item_create(int id,
	char* type,
	char* manufacturer,
	char* model,
	double price,
	int quantity)
{
	size_t type_size = strlen(type);
	char* type_copy = (char*)calloc(type_size + 1, sizeof(char));
	if (type_copy == NULL)
	{
		perror("Out of heap memory\n");
		exit(1);
	}
	strcpy_s(type_copy, type_size + 1, type);

	size_t manufacturer_size = strlen(manufacturer);
	char* manufacturer_copy = (char*)calloc(manufacturer_size + 1, sizeof(char));
	if (manufacturer_copy == NULL)
	{
		perror("Out of heap memory\n");
		exit(1);
	}
	strcpy_s(manufacturer_copy, manufacturer_size + 1, manufacturer);

	size_t model_size = strlen(model);
	char* model_copy = (char*)calloc(model_size + 1, sizeof(char));
	if (model_copy == NULL)
	{
		perror("Out of heap memory\n");
		exit(1);
	}
	strcpy_s(model_copy, model_size + 1, model);
	shop_item_t* item = (shop_item_t*)calloc(1, sizeof(shop_item_t));
	if (item == NULL)
	{
		perror("Out of heap memory\n");
		exit(1);
	}

	item->id = id;
	item->type = type_copy;
	item->manufacturer = manufacturer_copy;
	item->model = model_copy;
	item->price = price;
	item->quantity = quantity;

	return item;
}

void shop_item_destroy(shop_item_t* item)
{
	free(item->type);
	free(item->manufacturer);
	free(item->model);

	item->type = NULL;
	item->manufacturer = NULL;
	item->model = NULL;

	free(item);
	item = NULL;
}

shop_item_t* shop_item_copy(shop_item_t* other)
{
	return shop_item_create(other->id,
		other->type,
		other->manufacturer,
		other->model,
		other->price,
		other->quantity);
}

int shop_item_have_same_type_manufacturer_model(shop_item_t* item1, shop_item_t* item2)
{
	if (strcmp(item1->type, item2->type))
		return false;
	if (strcmp(item1->manufacturer, item2->manufacturer))
		return false;
	if (strcmp(item1->model, item2->model))
		return false;
	return true;
}

int	shop_item_get_id(shop_item_t* item)
{
	return item->id;
}

char* shop_item_get_type(shop_item_t* item)
{
	return item->type;
}

char* shop_item_get_manufacturer(shop_item_t* item)
{
	return item->manufacturer;
}

char* shop_item_get_model(shop_item_t* item)
{
	return item->model;
}

double shop_item_get_price(shop_item_t* item)
{
	return item->price;
}

int	shop_item_get_quantity(shop_item_t* item)
{
	return item->quantity;
}

void shop_item_increase_quantity_by(shop_item_t* item, int value)
{
	item->quantity += value;
}

void shop_item_set_quantity(shop_item_t* item, int new_quantity)
{
	item->quantity = new_quantity;
}

void shop_item_set_price(shop_item_t* item, double new_price)
{
	item->price = new_price;
}

int shop_item_has_id(shop_item_t* item, int* id)
{
	return shop_item_get_id(item) == *id;
}

int shop_item_is_manufactured_by(shop_item_t* item, char* manufacturer)
{
	return !strcmp(shop_item_get_manufacturer(item), manufacturer);
}

int shop_item_has_price_smaller_than(shop_item_t* item, double* price)
{
	return (shop_item_get_price(item) - *price) <= 0.0;
}

int shop_item_has_price_greater_than(shop_item_t* item, double* price)
{
	return (shop_item_get_price(item) - *price) >= 0.0;
}

int shop_item_has_quantity_smaller_than(shop_item_t* item, int* quantity)
{
	return shop_item_get_quantity(item) <= *quantity;
}

int shop_item_has_quantity_greater_than(shop_item_t* item, int* quantity)
{
	return shop_item_get_quantity(item) >= *quantity;
}

int shop_item_compare_by_price_ascending(shop_item_t* item1, shop_item_t* item2)
{
	if (item1->price <= item2->price)
		return true;
	return false;
}

int shop_item_compare_by_price_descending(shop_item_t* item1, shop_item_t* item2)
{
	return !shop_item_compare_by_price_ascending(item1, item2);
}

int shop_item_compare_by_quantity_ascending(shop_item_t* item1, shop_item_t* item2)
{
	return item1->quantity <= item2->quantity;
}

int shop_item_compare_by_quantity_descending(shop_item_t* item1, shop_item_t* item2)
{
	return item1->quantity > item2->quantity;
}
