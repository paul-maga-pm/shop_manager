#include "ShopService.h"

#include <stdlib.h>
#include <limits.h>
#include <string.h>


#include "Integer.h"
#include "Constants.h"


struct shop_service_struct
{
	list_t* used_id_list;
	list_t* stock_list;
};

shop_service_t* shop_service_create()
{
	shop_service_t* service = (shop_service_t*)calloc(1, sizeof(shop_service_t));
	if (!service)
	{
		perror("Out of heap memory");
		exit(1);
	}
	service->used_id_list = list_create(integer_copy, integer_destroy);
	service->stock_list = list_create(shop_item_copy, shop_item_destroy);
	return service;
}

void shop_service_destroy(shop_service_t* service)
{
	list_destroy(service->used_id_list);
	list_destroy(service->stock_list);
	free(service);
	service = NULL;
}



int shop_service_find_available_id(shop_service_t* service)
{
	int id;
	for (id = 1; id < INT_MAX; id++)
	{
		list_iterator_t* iterator = list_find_if(service->stock_list, &id, shop_item_has_id);
		if (!list_iterator_is_valid(iterator))
		{
			list_iterator_destroy(iterator);
			break;
		}
		list_iterator_destroy(iterator);
	}
	return id;
}


add_shop_item_result_t shop_service_add_item_in_stock(shop_service_t* service,
	shop_item_validation_result_t* validation_result,
	char* type,
	char* manufacturer,
	char* model,
	double price,
	int quantity)
{
	add_shop_item_result_t result;
	shop_item_t* item = shop_item_create(shop_service_find_available_id(service),
		type,
		manufacturer,
		model,
		price,
		quantity);
	shop_item_validation_result_initialize(validation_result);
	shop_item_validation_result_validate_shop_item(item, validation_result);
	
	if (!shop_item_validation_result_is_shop_item_valid(validation_result))
		result = ADD_INVALID;
	else
	{
		list_iterator_t* iterator_to_existing_item = list_find_if(service->stock_list, 
			item, 
			shop_item_have_same_type_manufacturer_model);

		if (!list_iterator_is_valid(iterator_to_existing_item))
		{
			list_add(service->stock_list, item);
			result = ADD_ADDED;
		}
		else
		{
			shop_item_increase_quantity_by(list_iterator_get(iterator_to_existing_item), quantity);
			result = ADD_INCREASED;
		}
		list_iterator_destroy(iterator_to_existing_item);
	}
	shop_item_destroy(item);
	return result;
}

update_shop_item_result_t shop_service_update_item(shop_service_t* service,
	int id, 
	shop_item_validation_result_t* validation_result, 
	double new_price, 
	int new_quantity)
{
	update_shop_item_result_t result;

	shop_item_t* item = shop_item_create(id, "type", "manufacturer", "model", new_price, new_quantity);
	shop_item_validation_result_validate_shop_item(item, validation_result);
	if (!shop_item_validation_result_is_shop_item_valid(validation_result))
		result = UPDATE_INVALID;
	else
	{
		list_iterator_t* iterator_to_item = list_find_if(service->stock_list, &id, shop_item_has_id);

		if (list_iterator_is_valid(iterator_to_item))
		{
			shop_item_t* existing_item = list_iterator_get(iterator_to_item);
			shop_item_set_price(existing_item, new_price);
			shop_item_set_quantity(existing_item, new_quantity);
			result = UPDATE_UPDATED;
		}
		else
			result = UPDATE_NOT_FOUND;
		list_iterator_destroy(iterator_to_item);
	}
	shop_item_destroy(item);
	return result;
}


int shop_service_remove_item_by_id(shop_service_t* service, int id)
{
	return list_remove_if(service->stock_list, &id, shop_item_has_id);
}

list_t* shop_service_get_stock(shop_service_t* service)
{
	return list_copy(service->stock_list);
}

list_t* shop_service_find_items_of_manufacturer(shop_service_t* service, char* manufacturer)
{
	return list_copy_if(service->stock_list, manufacturer, shop_item_is_manufactured_by);
}

list_t* shop_service_find_items_with_price_smaller_than(shop_service_t* service, double* price)
{
	return list_copy_if(service->stock_list, price, shop_item_has_price_smaller_than);
}

list_t* shop_service_find_items_with_price_greater_than(shop_service_t* service, double* price)
{
	return list_copy_if(service->stock_list, price, shop_item_has_price_greater_than);
}

list_t* shop_service_find_items_with_quantity_smaller_than(shop_service_t* service, int* quantity)
{
	return list_copy_if(service->stock_list, quantity, shop_item_has_quantity_smaller_than);
}

list_t* shop_service_find_items_with_quantity_greater_than(shop_service_t* service, int* quantity)
{
	return list_copy_if(service->stock_list, quantity, shop_item_has_quantity_greater_than);
}

list_t* shop_service_sort_items_by_price_ascending(shop_service_t* service)
{
	list_t* sorted_items = list_copy(service->stock_list);
	list_sort(sorted_items, shop_item_compare_by_price_ascending);
	return sorted_items;
}

list_t* shop_service_sort_items_by_price_descending(shop_service_t* service)
{
	list_t* sorted_items = list_copy(service->stock_list);
	list_sort(sorted_items, shop_item_compare_by_price_descending);
	return sorted_items;
}

list_t* shop_service_sort_items_by_quantity_ascending(shop_service_t* service)
{
	list_t* sorted_items = list_copy(service->stock_list);
	list_sort(sorted_items, shop_item_compare_by_quantity_ascending);
	return sorted_items;
}

list_t* shop_service_sort_items_by_quantity_descending(shop_service_t* service)
{
	list_t* sorted_items = list_copy(service->stock_list);
	list_sort(sorted_items, shop_item_compare_by_quantity_descending);
	return sorted_items;
}





