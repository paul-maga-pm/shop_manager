#ifndef SHOP_SERVICE_H
#define SHOP_SERVICE_H

#include "ShopItemValidator.h"
#include "List.h"

typedef struct shop_service_struct shop_service_t;

typedef enum
{
	ADD_ADDED,
	ADD_INCREASED,
	ADD_INVALID
}add_shop_item_result_t;

shop_service_t*				shop_service_create();
void						shop_service_destroy(shop_service_t* service);

add_shop_item_result_t		shop_service_add_item_in_stock(shop_service_t* service,
							shop_item_validation_result_t* validation_result,
							char* type,
							char* manufacturer,
							char* model,
							double price,
							int quantity);


typedef enum
{
	UPDATE_INVALID,
	UPDATE_NOT_FOUND,
	UPDATE_UPDATED
}update_shop_item_result_t;

update_shop_item_result_t	shop_service_update_item(shop_service_t* service,
							int id,
							shop_item_validation_result_t* validation_result,
							double new_price,
							int new_quantity);

int							shop_service_remove_item_by_id(shop_service_t* service, int id);
list_t*						shop_service_get_stock(shop_service_t* service);

list_t*						shop_service_find_items_of_manufacturer(shop_service_t* service, char* manufacturer);

list_t*						shop_service_find_items_with_price_smaller_than(shop_service_t* service, double* price);
list_t*						shop_service_find_items_with_price_greater_than(shop_service_t* service, double* price);

list_t*						shop_service_find_items_with_quantity_smaller_than(shop_service_t* service, int* quantity);
list_t*						shop_service_find_items_with_quantity_greater_than(shop_service_t* service, int* quantity);

list_t*						shop_service_sort_items_by_price_ascending();
list_t*						shop_service_sort_items_by_price_descending();
list_t*						shop_service_sort_items_by_quantity_ascending();
list_t*						shop_service_sort_items_by_quantity_descending();
#endif 